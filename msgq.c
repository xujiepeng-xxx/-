#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msgq.h"

int get_msg(const char *pathname, int proj_id)
{
	int msgid = msgget(ftok(pathname,proj_id),IPC_CREAT|0644);
	if(0 > msgid)
	{
		perror("msgget");
	}
	return msgid;
}

int send_msg(int msgid,const void* msg,size_t len)
{
	int ret = msgsnd(msgid,msg,len-sizeof(long),0);
	if(0 > ret)
	{
		perror("msgsnd");
	}
	return ret;
}

int recv_msg(int msgid,void* msg,size_t len,long type)
{
	int ret = msgrcv(msgid,msg,len,type,0);
	if(0 >= ret)
	{
		perror("msgrcv");
	}
	return ret;
}

void del_msg(int msgid)
{
	if(msgctl(msgid,IPC_RMID,NULL))
	{
		perror("msgctl");
	}
}
