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
int login(void)
{
	//msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	int fd = open("user.txt",O_RDWR,0644);		//打开文件
	if(0 > fd)								//判断文件有没有打开成功
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
		sscanf(buf,"%s%s%s%lf%hhd\n",temp[max_cnt].id,temp[max_cnt].pass,temp[max_cnt].card,&temp[max_cnt].balance,&temp[max_cnt].islock);
		max_cnt++;
		buf = strstr(buf,"\n")+1;
	}
	free(temp_fd);
	for(int i = 0;i <max_cnt;i++)		
	{
		if(0 == strcmp(ctos.acc.id,temp[i].id))		//寻找要登录的用户id
		{
			if(1 == temp[i].islock)return -3;       //账号被锁定
			if(0 == strcmp(ctos.acc.pass,temp[i].pass)) //判断密码是否正确
			{
				return 1;  //账号、密码都正确
			}
			else
			{
				for(int j=0;j<2;j++)
				{
					sprintf(stoc.info,"F");			//要传递的信息
					stoc.type = ctos.return_type;	//返回的路径
					//发送消息
					send_msg(msgid,&stoc,sizeof(MSG_STOC));
					//接收消息
					recv_msg(msgid,&ctos,sizeof(MSG_CTOS),MSGTY_LOGIN);
					if(0 == strcmp(ctos.acc.pass,"quit"))return 0;
					if(0 == strcmp(ctos.acc.pass,temp[i].pass)) //判断密码是否正确
					{
						close(fd);
						return 1;  //账号、密码都正确
					}					
				}
				ftruncate(fd,0);			//清空文件内容
				lseek(fd,0,SEEK_SET);		//将文件位置指针放到开头
				temp[i].islock = 1;      //将标志位置为 1 ，表示已锁定
				for(int j = 0;j < max_cnt;j++)		//写入信息
				{
					char str[256]={};
					sprintf(str,"%s %s %s %lf %hhd\n",temp[j].id,temp[j].pass,temp[j].card,temp[j].balance,temp[j].islock);
					write(fd,str,strlen(str));
				}
				close(fd);   //关闭文件
				return -1;  //表示密码已经输入错误3次
			}
		}
	}
	close(fd);
	return -2;  //表示账号不存在
}

int main(int argc,const char* argv[])
{
	//获得消息队列
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	for(;;)
	{
		//接收消息
		recv_msg(msgid,&ctos,sizeof(MSG_CTOS),MSGTY_LOGIN);
		int ret = login();
		if(1 == ret)
		{
			sprintf(stoc.info,"Y");
			stoc.type = ctos.return_type;
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
		else if(-1 == ret)
		{
			sprintf(stoc.info,"密码输入错误已经3次，已锁定！");
			stoc.type = ctos.return_type;
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
		else if(-2 == ret)
		{
			sprintf(stoc.info,"账号不存在！");
			stoc.type = ctos.return_type;
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
		else if(-3 == ret)
		{
			sprintf(stoc.info,"账号已经被锁定，请前去解锁！");
			stoc.type = ctos.return_type;
			send_msg(msgid,&stoc,sizeof(MSG_STOC));
		}
	}	
}
