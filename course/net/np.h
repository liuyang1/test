//	liuyang1,2011-11-24 18:25:23
//	GUARD support
#ifndef _NP_H_
#define _NP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <event.h>
#include <pthread.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/util.h>

#define USERMAXNUM	10


#define MAXBUF		sizeof(struct ChatMsg)		//length of buffer
#define SERVERPORT	10240				//server port
#define PROTOCOL	SOCK_STREAM			//udp link/tcp link
										//SOCK_STREAM SOCK_DGRAM

#define _TEST
#define _SSL

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
#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

struct ChatMsg{
	char fromUser[10];
	char toUser[10];
	char mBuf[1024];
};

struct Client{
	char	name[10];
	int		sockfd;
	SSL*	sslfd;
	struct sockaddr addr;
	struct bufferevent *buf_ev;
};

struct CtrlMsg{
	char act;
	char msg[1024];
};
void error_printf(char *str);
void test_printf(char *str);
inline void info_printf(char *str){
#ifdef _TEST
	printf("[INFO ] %s\n",str);
#endif
}

void CharMsg2msg(struct ChatMsg *src,char * msg);

void msg2CharMsg(char *msg,struct ChatMsg *src);

int setnonblock(int fd);

int initSSL();

#endif
