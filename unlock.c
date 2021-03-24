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

int unlock(void)
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
	for(int i = 0;i <max_cnt;i++)	//判断要解锁的账号存不存在
	{
		if(0 == strcmp(ctos.acc.id,temp[i].id))	//找到了要解锁的ID
		{	
			if(0 == strcmp(ctos.acc.pass,temp[i].pass))	//判断密码是否一致
			{
				ftruncate(fd,0);			//清空文件内容
				lseek(fd,0,SEEK_SET);		//将文件位置指针放到开头
				temp[i].islock = 0;      //将标志位置为 0 ，表示已解锁
				for(int j = 0;j < max_cnt;j++)		//写入信息
				{
					char str[256]={};
					sprintf(str,"%s %s %s %lf %hhd\n",temp[j].id,temp[j].pass,temp[j].card,temp[j].balance,temp[j].islock);
					write(fd,str,strlen(str));
				}
				close(fd);   //关闭文件
				return 1;		//返回解锁成功的信号
			}
			else
			{
				close(fd);   //关闭文件
				return -1;   //密码不正确退出
			}
		}
	}
	close(fd);			//关闭文件
	return -1;			//找不到要解锁的ID，退出
}

int main(int argc,const char* argv[])
{
	//获得消息队列
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	for(;;)
	{
		//接受信息
		recv_msg(msgid,&ctos,sizeof(MSG_CTOS),MSGTY_UNLOCK);
		if(1 == unlock())
		{
			strcpy(stoc.info,"解锁成功");
		}
		else
			strcpy(stoc.info,"解锁失败");
		stoc.type = ctos.return_type;
		//发送信息
		send_msg(msgid,&stoc,sizeof(MSG_STOC));
	}	
}
