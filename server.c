#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "bank.h"

pid_t subp[SUBP_CNT] = {};

void sigint(int signum)
{
	for(int i=0; i<SUBP_CNT; i++)
	{
		kill(subp[i],SIGINT);
	}
	exit(0);
}

int main()
{
	// 绑定信号处理
	signal(SIGINT,sigint);
	
	// 启动子进程
	const char* paths[SUBP_CNT] = {
		"./bin/open",
		"./bin/destory",
		"./bin/unlock",
		"./bin/login",
		"./bin/query",
		"./bin/save",
		"./bin/take",
		"./bin/transfer",
		"./bin/repass",
	};
	for(int i=0; i<SUBP_CNT; i++)
	{
		subp[i] = vfork();
		if(0 == subp[i])
		{
			execl(paths[i],paths[i],NULL);
		}
	}
	
	// 等待子进程结束
	while(-1 != wait(NULL));
}
