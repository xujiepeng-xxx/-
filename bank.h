#ifndef BANK_H
#define BANK_H
#include <unistd.h>

#define MSGTY_OPEN		(1)
#define MSGTY_DESTORY	(2)
#define MSGTY_UNLOCK	(3)
#define MSGTY_LOGIN		(4)
#define MSGTY_QUERY		(5)
#define MSGTY_SAVE		(6)
#define MSGTY_TAKE		(7)
#define MSGTY_TRANSFER	(8)
#define MSGTY_REPASS	(9)

#define SUBP_CNT 		(9)
#define MAX_CNT 		50      //最大人数
#define ACCOUNT_PATH 	"./account/"
#define ID_PATH 		"./account/.id.dat"


typedef struct Account		//用户数据结构体
{
	double balance;			//余额
	char id[20];				//用户ID
	char pass[7];			//用户密码
	char card[19];			//用户身份证
	char islock;				//是否被锁定的标志
}Account;

typedef struct MSG_CTOS		//客户端传输给服务端的结构体
{	
	long type;					//信息类型（消息队列）
	Account acc;					//要传输的用户信息
	pid_t return_type;			//客户端进程PID
}MSG_CTOS;

typedef struct MSG_STOC		//服务端返回给客户端的信息
{		
	long type;					//信息类型（消息队列）即客户端的PID
	char info[256];				//返回的消息
}MSG_STOC;

#endif//BANK_H
