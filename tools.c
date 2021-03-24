#include "tools.h"
#include <getch.h>

int get_cmd(char start,char end)		//获取输入的指令
{
	puts("--------------------");
	printf("请输入指令：");
	for(;;)
	{
		char cmd = getch();
		if(start <= cmd && cmd <= end)
		{
			printf("%c\n",cmd);
			return cmd;
		}
		else
			printf("请输入正确的指令\n");
	}
}

int judge_len(char* str,int len)		//判断输入是否合法，len为最大长度,返回长度
{
	int cnt = 0;
	while(str[cnt++]);
	if(cnt-1 > len)
	{
		return -1;
	}
	return cnt-1;
}

void buffer_clear(void)			//清空缓冲区
{
	char ch;
	while((ch = getchar())!='\n' && ch!=EOF);
}


