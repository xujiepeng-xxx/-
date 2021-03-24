#include "msgq.h"
#include "bank.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>
int msgid = 0;
MSG_CTOS ctos = {};			//客户端传输给服务器的数据
MSG_STOC stoc = {};			//服务器返回的数据
void sleep_show(int time)  //停止一端时间
{
	sleep(time);
}
int main_menu(void)			//主界面的显示
{
	
	system("clear");
	puts("***欢迎使用C/S银行***");
	puts("1、开户	2、销户");
	puts("3、解锁	4、登录");
	puts("5、退卡");
	return get_cmd('1','5');		//get_cmd(1,5) 限定的输入的值的范围和获取输入的指令
}

int judge_card(void)			//判断身份证的输入合不合法
{
	for(;;)
	{	
		printf("请输入你的身份证号码：");
		scanf("%s",ctos.acc.card);		//用户身份证
		if(judge_len(ctos.acc.card,18) == 18)
		{
			return 1;
		}
		else
		{
			printf("输入格式不正确，退出请输入quit\n");
			if(0 == strcmp(ctos.acc.card,"quit"))
			{
				buffer_clear();				//清空缓冲区
				printf("正在退出中。。。\n");
				sleep(3);
				return -1;
			}
		}
	}
}
void open_acc(void)						//开户功能
{
	// 准备数据
	//sprintf(ctos.acc.card,"open");
	//if(-1 == judge_card())return;	//用户身份证的输入，judge_card()验证合法性
	//ctos.acc.balance = 10;	
	printf("请输入你的身份证号码：");
	scanf("%s",ctos.acc.card);		//用户身份证			
	printf("请输入你开户时要充值的金额:");
	scanf("%lf",&ctos.acc.balance);	//账户的余额
	getchar();
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_OPEN; 			//信息的类型
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收服务端的信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}

void destory(void)			//销户功能	
{
	// 准备数据
	printf("请输入你要销毁账户的id\n");
	scanf("%s",ctos.acc.id);				//输入用户ID
	printf("请输入你要销毁账户的密码\n");
	scanf("%s",ctos.acc.pass);			//输入用户密码
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_DESTORY; 			//信息的类型
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收服务端的信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}

void unlock(void)			//解锁功能
{
	// 准备数据
	printf("请输入你要解锁账户的id\n");
	scanf("%s",ctos.acc.id);				//输入用户ID
	printf("请输入你要解锁账户的密码\n");
	scanf("%s",ctos.acc.pass);			//输入用户密码
	ctos.return_type = getpid();		   //服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_UNLOCK; 			//信息的类型
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收服务端的信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}

int sub_menu(void)			//次级界面的显示	
{
	system("clear");
	puts("****用户界面****");
	printf("ID:%s\n",ctos.acc.id);
	puts("1、查询	2、存款");
	puts("3、取款	4、转账");
	puts("5、改密	6、退卡");
	return get_cmd('1','6');		//get_cmd(1,6) 限定的输入的值的范围和获取输入的指令
}


void query(void)			//查询功能
{
	// 准备数据
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_QUERY; 			//信息的类型
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收服务端信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}

void save(void)				//存款功能
{
	// 准备数据
	printf("请输入你要储存的金额：");
	scanf("%lf",&ctos.acc.balance);
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_SAVE; 			//信息的类型
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收服务端信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}

void take(void)				//取款功能
{
	// 准备数据
	printf("请输入你要提取的金额：");
	scanf("%lf",&ctos.acc.balance);
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_TAKE; 			//信息的类型
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收服务端信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}

void transfer(void)			//转帐功能
{
	char str_id[20] = {};
	strcpy(str_id,ctos.acc.id);
	// 准备数据
	printf("请输入你要转账的金额：");
	scanf("%lf",&ctos.acc.balance);  //要转账的金额
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_TRANSFER; 			//信息的类型
	//发送信息给服务端，接收服务器发来的信息。
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	scanf("%s",ctos.acc.id);
	//发送信息给服务端，接收服务器发来的信息。
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	strcpy(ctos.acc.id,str_id);
	sleep_show(3);
}

void repass(void)			//改密功能
{
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	ctos.type = MSGTY_REPASS; 			//信息的类型
	// 准备数据
	printf("请输入你要修改的密码：");
	scanf("%s",ctos.acc.pass);
	//发送信息给服务端，第一次输入密码
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	//第二次输入密码
	scanf("%s",ctos.acc.pass);
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	// 根据返回结果给用户显示信息
	printf("%s\n",stoc.info);
	sleep_show(3);
}
void login(void)		//登陆功能
{
	ctos.type = MSGTY_LOGIN; 			   //信息的类型
	ctos.return_type = getpid();		//服务端返回的路径，即客户端的PID
	//判断登陆有没有成功
	printf("请输入账户的id\n");
	scanf("%s",ctos.acc.id);				//输入用户ID
	printf("请输入账户的密码\n");
	scanf("%s",ctos.acc.pass);			//输入用户密码
	//发送信息给服务端
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	//接收信息
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	while(0 == strcmp(stoc.info,"F"))
	{
		printf("密码错误请再次输入,\"quit\"退出!\n");
		scanf("%s",ctos.acc.pass);			//输入用户密码
		if(0 == strcmp(stoc.info,"quit"))return;
		//发送信息给服务端
		send_msg(msgid,&ctos,sizeof(MSG_CTOS));
		//接收信息
		recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	}
	if(0 != strcmp(stoc.info,"Y"))
	{
		printf("%s\n",stoc.info);
		sleep_show(3);
		return;
	}
	printf("登录成功\n");
	sleep_show(3);
	for(;;)
	{
		switch(sub_menu())
		{
			case '1': query(); break;
			case '2': save(); break;
			case '3': take(); break;
			case '4': transfer(); break;
			case '5': repass(); break;
			case '6': exit(0);
		}
	}
}

int main()
{
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);
	for(;;)
	{
		switch(main_menu())
		{
			case '1': open_acc(); break;
			case '2': destory(); break;
			case '3': unlock(); break;
			case '4': login(); break;
			case '5': return 0;
		}

	}
	
	/*
	msgid = get_msg(ACCOUNT_PATH,SUBP_CNT);

	ctos.acc.balance = 10;
	ctos.return_type = getpid();
	
	ctos.type = MSGTY_OPEN;
	sprintf(ctos.acc.card,"open");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	
	ctos.type = MSGTY_DESTORY;
	sprintf(ctos.acc.card,"destory");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);

	ctos.type = MSGTY_UNLOCK;
	sprintf(ctos.acc.card,"unlock");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	
	ctos.type = MSGTY_LOGIN;
	sprintf(ctos.acc.card,"LOGIN");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	*/
	
	/*ctos.type = MSGTY_QUERY;
	sprintf(ctos.acc.card,"QUERY");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);

	ctos.type = MSGTY_SAVE;
	sprintf(ctos.acc.card,"SAVE");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	
	ctos.type = MSGTY_TAKE;
	sprintf(ctos.acc.card,"TAKE");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	
	ctos.type = MSGTY_TRANSFER;
	sprintf(ctos.acc.card,"TRANSFER");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	
	ctos.type = MSGTY_REPASS;
	sprintf(ctos.acc.card,"REPASS");
	send_msg(msgid,&ctos,sizeof(MSG_CTOS));
	recv_msg(msgid,&stoc,sizeof(MSG_STOC),ctos.return_type);
	printf("%s\n",stoc.info);
	*/
	
}
