/*
 * ========================================================
 *
 *       Filename:  broadcast.c
 *
 *    Description:  An IP multicast server
 *
 *        Version:  1.0
 *        Created:  06/02/2013 05:13:19 PM
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

int main(){
    int sockd;
    sockd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockd==-1){
        perror("open socket");
        exit(EXIT_FAILURE);
    }

    char hostname[]="224.0.0.1";
    int port=5000;
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(hostname);
    addr.sin_port=htons(port);

    char str[]="test from broadcast";
    int cnt=0;
    while(1){
        if(sendto(sockd,str,sizeof(str),0,(struct sockaddr *)&addr,sizeof(addr))<0){
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        printf("-");
        if((++cnt)==9){
            cnt=0;
            printf("\n");
        }
        fflush(stdout);
    }
    return EXIT_SUCCESS;
}
