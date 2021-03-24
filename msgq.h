#ifndef MSGQ_H
#define MSGQ_H
#include <stdio.h>

int get_msg(const char *pathname, int proj_id);
int send_msg(int msgid,const void* msg,size_t len);
int recv_msg(int msgid,void* msg,size_t len,long type);
void del_msg(int msgid);
 
#endif//MSGQ_H
