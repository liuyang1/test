#ifndef _NP_H_
#define _NP_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <pthread.h>

//#define _TEST

inline void error_printf(char *str){
#ifdef _TEST
	printf("[ERROR] %s\n",str);
#endif
}
inline void test_printf(char *str){
#ifdef _TEST
	printf("[SUCC ] %s\n",str);
#endif
}

#define GUARD(express)				\
{									\
	int ret=express;				\
	if(ret<0){						\
		error_printf(#express);		\
		exit(-1);					\
	}else{							\
		test_printf(#express);		\
	}								\
}

struct ChatMsg{
	char fromUser[10];
	char toUser[10];
	char mBuf[1024];
};

struct UserInfo{
	char	name[10];
	int		sockfd;
	struct sockaddr addr;
};

struct CtrlMsg{
	char act;
	char msg[1024];
};

#define USERMAXNUM	10

#define MAXBUF		sizeof(struct ChatMsg)		//length of buffer
#define SERVERPORT	10240				//server port
#define PROTOCOL	SOCK_STREAM			//udp link/tcp link
										//SOCK_STREAM SOCK_DGRAM
inline int chkbye(char *str){
	if(!strcpy(str,"bye"))
		exit(0);
	return 0;
}

inline void CharMsg2msg(struct ChatMsg *src,char * msg){
	sprintf(msg,"%10s%10s%1024s",src->fromUser,src->toUser,src->mBuf);
}

inline void msg2CharMsg(char *msg,struct ChatMsg *src){
	strcpy(src->fromUser,msg);
	strcpy(src->toUser,msg+10);
	strcpy(src->mBuf,msg+20);
}

//inline void CtrlMsg2msg(struct CtrlMsg *src,char *msg){
//	sprintf(msg,"%c%1024s",msg
//intline void AddUser2Ctrl(
#endif
