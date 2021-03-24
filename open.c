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
void rand_id(char* id)		//随机生成用户ID
{
	for(int i=0;i<18;i++)
	{
		id[i] = rand()%10+48;
	}
}
int open_acc(void)
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
		sscanf(buf,"%s%s%s%lf%hhd\n",temp[max_cnt].id,temp[max_cnt].pass,temp[max_cnt].card,&temp[max_cnt].balance,&temp[max_cnt].islock);
		printf("%s %s %s %lf %hhd\n",temp[max_cnt].id,temp[max_cnt].pass,temp[max_cnt].card,temp[max_cnt].balance,temp[max_cnt].islock);
		max_cnt++;
		buf = strstr(buf,"\n")+1;
		
	}
	free(temp_fd);
	for(int i = 0;i <max_cnt;i++)
	{
		printf("%d\n",strcmp(ctos.acc.card,temp[i].card));
		if(0 == strcmp(ctos.acc.card,temp[i].card))
		{
			return -1;
		}
	}
	rand_id(temp[max_cnt].id);			//随机生成用户ID
	strcpy(temp[max_cnt].pass,"000000");
	char str[256]={};
	sprintf(str,"%s %s %s %lf %hhd\n",temp[max_cnt].id,temp[max_cnt].pass,ctos.acc.card,ctos.acc.balance,0);
	printf("%s %s %s %lf %hhd\n",temp[max_cnt].id,temp[max_cnt].pass,ctos.acc.card,ctos.acc.balance,0);
	write(fd,str,strlen(str));
	close(fd);
	return 1;
}

int main(int argc,const char* argv[])
{
	srand(time(NULL));
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);		//获取消息对列
	for(;;)
	{
		//接收信息
		recv_msg(msgid,&ctos,sizeof(MSG_CTOS),MSGTY_OPEN);
		printf("%s\n",ctos.acc.card);		
		if(1 == open_acc())
		{
			strcpy(stoc.info,"开户成功");
		}
		else 
			strcpy(stoc.info,"开户失败");
		stoc.type = ctos.return_type;
		//发送信息
		send_msg(msgid,&stoc,sizeof(MSG_STOC));
	}	
}
