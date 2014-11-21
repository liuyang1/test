/*
 * ========================================================
 *
 *       Filename:  res.c
 *
 *    Description:  respone for mulicast broadcast program
 *
 *        Version:  1.0
 *        Created:  06/02/2013 05:34:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main(void){
    int loop=1;

    int sockd;
    if((sockd=socket(PF_INET,SOCK_DGRAM,0))==-1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    loop=1;
    if(setsockopt(sockd,SOL_SOCKET,SO_REUSEADDR,&loop,sizeof(loop))<0){
        perror("setsockopt:SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sin;
    int port=5000;
    bzero(&sin,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=htonl(INADDR_ANY);
    sin.sin_port=htons(port);

    if(bind(sockd,(struct sockaddr *)&sin,sizeof(sin))<0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    loop=1;
    if(setsockopt(sockd,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop))<0){
        perror("setsockopt:IP_MULTICAST_LOOP");
        exit(EXIT_FAILURE);
    }

    struct ip_mreq cmd;
    char hostname[]="224.0.0.1";
    cmd.imr_multiaddr.s_addr=inet_addr(hostname);
    cmd.imr_interface.s_addr=htonl(INADDR_ANY);
    if(cmd.imr_multiaddr.s_addr==-1){
        perror("not a legal multicast addr");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(sockd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&cmd,sizeof(cmd))<0){
        perror("setsockopt:IP_ADD_MEMBERSHIP");
    }

    int iter=0;
#define BUFLEN  256
    char message[BUFLEN];
    int sin_len;
    while(iter++<8){
        sin_len=sizeof(sin);
        if(recvfrom(sockd,message,BUFLEN,0,(struct sockaddr *)&sin,&sin_len)==-1){
            perror("recvfrom");
        }
        printf("resopnse: #%-2d from server : %s\n",iter,message);
        sleep(1);
    }

    if(setsockopt(sockd,IPPROTO_IP,IP_DROP_MEMBERSHIP,&cmd,sizeof(cmd))<0){
        perror("setsockopt:IP_DROP_MEMBERSHIP");
    }
    close(sockd);
    exit(EXIT_SUCCESS);
}
