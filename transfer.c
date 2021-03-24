#include "msgq.h"
#include "bank.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int msgid = 0;
MSG_CTOS ctos = {};
MSG_STOC stoc = {};

int transfer(void)
{
	int fd = open("user.txt",O_RDWR,0644);		//打开文件
	if(0 > fd)
	{
		perror("open");
		return -1;
	}
	Account temp[MAX_CNT] = {};			//用来记录文件中的内容
	char* buf = malloc(4096);				//缓冲区
	char* temp_fd = buf;					//记录，用来释放内存
	int max_cnt = 0;						//记录从文件中读取的信息个数
	if(0 > read(fd,buf,4096))				//把文件中所有数据都读到buf中		
	{
		perror("read");
		return -1;
	}
	
	while(strstr(buf,"\n"))			//从文件中读写
	{
		sscanf(buf,"%s%s%s%lf%hhd\n",temp[max_cnt].id,temp[max_cnt].pass,temp[max_cnt].card,&temp[max_cnt].balance,&temp[max_cnt].islock);			//从buf中按格式读取信息
		max_cnt++;
		buf = strstr(buf,"\n")+1;	//跳到下一条信息前的位置
	}
	free(temp_fd);						//释放buf申请的内存
	
	for(int i = 0;i <max_cnt;i++)	//找到要取款的账号
	{
		if(0 == strcmp(ctos.acc.id,temp[i].id))	//找到了要去转账的ID
		{	
			
			temp[i].balance = temp[i].balance - ctos.acc.balance; //将金额减去
			if(0 > temp[i].balance) return -1;		//提取的金额过大
			
			sprintf(stoc.info,"请输入你要转账对象的ID！");
			//返回路径
			stoc.type = ctos.return_type;
			//发送信息并接收
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
			recv_msg(msgid,&ctos,sizeof(MSG_CTOS),MSGTY_TRANSFER);
			printf("ctos.acc.id:%s\n",ctos.acc.id);
			
			for(int j = 0;j <max_cnt;j++)	//找到要转账的账号
			{
				if(0 == strcmp(ctos.acc.id,temp[j].id))	//找到了要转账的ID
				{
					temp[j].balance = temp[j].balance + ctos.acc.balance; //将金额加上
					ftruncate(fd,0);			//清空文件内容
					lseek(fd,0,SEEK_SET);		//将文件位置指针放到开头
					for(int z = 0;z < max_cnt;z++)		//写入信息
					{
						char str[256]={};
						sprintf(str,"%s %s %s %lf %hhd\n",temp[z].id,temp[z].pass,temp[z].card,temp[z].balance,temp[z].islock);
						write(fd,str,strlen(str));
					}
					return 1;  //转账成功的标志
				}
			}
			close(fd);			//关闭文件
			return -2;     //转账失败的标志，找不到账号
		}
	}
	close(fd);			//关闭文件
	return -3;			//出现错误，退出
}

int main(int argc,const char* argv[])
{
	printf("argv %s\n",argv[0]);
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	
	for(;;)
	{
		recv_msg(msgid,&ctos,sizeof(MSG_CTOS),MSGTY_TRANSFER);
		if(0 > ctos.acc.balance)
		{
			sprintf(stoc.info,"输入错误，请输入正数！");
			//返回路径
			stoc.type = ctos.return_type;
			//发送信息
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
			continue;
		}
		int ret = transfer();
		if(1 == ret)
		{
			sprintf(stoc.info,"转账成功！");
			//返回路径
			stoc.type = ctos.return_type;
			//发送信息
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
		else if(-1 == ret)
		{
			sprintf(stoc.info,"转账的金额超出余额！");
			//返回路径
			stoc.type = ctos.return_type;
			//发送信息
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
		else if(-2 == ret)
		{
			sprintf(stoc.info,"寻找不到被转账的ID！");
			//返回路径
			stoc.type = ctos.return_type;
			//发送信息
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
		else if(-3 == ret)
		{
			sprintf(stoc.info,"系统错误！");
			//返回路径
			stoc.type = ctos.return_type;
			//发送信息
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
	}	
}
