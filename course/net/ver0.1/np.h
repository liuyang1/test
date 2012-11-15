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

#define MAXBUF	1024		//length of buffer
#define SERVERPORT	10240	//server port
#define PROTOCOL	SOCK_STREAM//udp link/tcp link
									//SOCK_STREAM SOCK_DGRAM
#endif
