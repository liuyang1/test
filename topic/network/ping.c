/*
 * ========================================================
 *
 *       Filename:  icmp.c
 *
 *    Description:  发送并且接受icmp数据包,就是和ping一样的程序
 *
 *        Version:  1.0
 *        Created:  06/01/2013 07:26:42 PM
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
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>

#define PACKET_SIZE 4096
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS  3

char sendpacket[PACKET_SIZE];
char recvpacket[PACKET_SIZE];
int sockfd,datalen=56;
int nsend=0,nreceived=0;
struct sockaddr_in dest_addr;
pid_t pid;
struct sockaddr_in from;
struct timeval tvrecv;

void statistics(int signo){
    printf("\n----PING stat----\n");
    printf("%d packets transmitted, %d received, %%%.2f lost\n",nsend,nreceived,(float)(nsend-nreceived)/(nsend*100.0));
    close(sockfd);
    exit(1);
    return;
}

unsigned short cal_chksum(unsigned short *addr,int len){
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short ans=0;
    while(nleft>1){
        sum+=*w++;
        nleft-=2;
    }
    if(nleft==1){
        *(unsigned char *)(&ans)=*(unsigned char *)w;
        sum+=ans;
    }
    sum=(sum>>16)+(sum&0xffff);
    sum+=(sum>>16);
    ans=~sum;
    return ans;
}

int pack(int pack_no){
    int packsize;
    struct icmp *icmp;
    /*
     * timeval
     * time_t tv_sec;       // SLONG
     * suseconds_t tv_usec; // U32
     */
    struct timeval *tval; // 8 bytes
    /* icmp packet
     * byte byte byte byte
     * Type Code  ChkSum
     *  rest of header
     * 在linux中,除了上述内容,还会填充数据是的总大小为64字节
     * 在windows中,则填充到40字节
     *
     * 常见消息列表
     * Type Code    意思
     * 0    0       echo reply
     * 3    *       目的地不可达
     * 5    *       重定向消息
     * 8    0       echo request
     *
     * for ICMP request,ICMP packet:
     * byte byte byte byte
     * Type Code  ChkSum
     *    ID       Seq
     *  Origin TimeStamp
     */
    icmp=(struct icmp*)sendpacket;
    icmp->icmp_type=ICMP_ECHO;
    icmp->icmp_code=0;
    icmp->icmp_cksum=0;
    icmp->icmp_seq=pack_no;
    icmp->icmp_id=pid;
    packsize=sizeof(struct icmp)+datalen;// 填充到64字节
    //const struct timeval * tval=(struct timveval *)icmp->icmp_data;
    tval=(struct timeval *)icmp->icmp_data;
    gettimeofday(tval,NULL);
    icmp->icmp_cksum=cal_chksum((unsigned short*)icmp,packsize);
    return packsize;
}

void send_packet(){
    int packetsize;
    nsend++;
    packetsize=pack(nsend);
    if(sendto(sockfd,sendpacket,packetsize,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr))<0){
        perror("sendto error");
        exit(-1);
    }
}

void tv_sub(struct timeval *out,struct timeval *in){
    if (( out->tv_usec-=in->tv_usec)<0){
        --out->tv_sec;
        out->tv_usec+=1000000;
    }
    out->tv_sec-=in->tv_sec;
}
int unpack(char* buf,int len){
    int iphdrlen;
    struct ip *ip;
    struct icmp *icmp;
    struct timeval *tvsend;
    struct timeval *MsgTimeRecv;
    struct timeval *MsgTimeTran;
    double rtt;
    ip=(struct ip *)buf;
    iphdrlen=ip->ip_hl<<2;
    icmp=(struct icmp *)(buf+iphdrlen);
    len-=iphdrlen;
    if(len<8){
        printf("ICMP packets\'s length is less than 8\n");
        return -1;
    }
    /*
     * for ICMP reply,ICMP packet:
     * byte byte byte byte
     * Type Code  ChkSum
     *    ID       Seq
     *  OriginTimeStamp
     *  ReceiveTimeStamp        // NOT IMPL???
     *  TransmitTimeStamp
     */
    if((icmp->icmp_type==ICMP_ECHOREPLY) && (icmp->icmp_id==pid)){
        tvsend=(struct timeval *)icmp->icmp_data;
        tv_sub(&tvrecv,tvsend);
        rtt=tvrecv.tv_sec*1000+tvrecv.tv_usec/1000.0;
        printf("%d byte from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
                len,inet_ntoa(from.sin_addr),icmp->icmp_seq,ip->ip_ttl,rtt);
        return 0;
    }
    else
        return -1;
}
void recv_packet(){
    int n;
    unsigned int fromlen;
    extern int errno;
    signal(SIGALRM,statistics);
    fromlen=sizeof(from);
    while(1){
        alarm(MAX_WAIT_TIME);
        if ( ( n=recvfrom(sockfd,recvpacket,sizeof(recvpacket),0,(struct sockaddr *)&from,&fromlen))<0){
            if(errno==EINTR)
                continue;
            perror("recvfrom error");
            continue;
        }
        gettimeofday(&tvrecv,NULL);
        if(unpack(recvpacket,n)==-1)
            continue;
        nreceived++;
        break;
    }
}

int main(int argc,char** argv){
    struct hostent *host;
    struct protoent *protocol;
    unsigned long inaddr=0l;
    //int waittime=MAX_WAIT_TIME;
    int size=50*1024;

    if(argc<2){
        printf("usage:%s hostname/IP address\n",argv[0]);
        exit(1);
    }

    if((protocol=getprotobyname("icmp"))==NULL){
        perror("getprotobyname");
        exit(1);
    }
    if((sockfd=socket(AF_INET,SOCK_RAW,protocol->p_proto))<0){
        perror("socket error");
        exit(1);
    }
    setuid(getuid());

    setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size));
    bzero(&dest_addr,sizeof(dest_addr));
    dest_addr.sin_family=AF_INET;

    if((inaddr=inet_addr(argv[1]))==INADDR_NONE){
        if((host=gethostbyname(argv[1]))==NULL){
            perror("gethostbyname error");
            exit(1);
        }
        memcpy((char*)&dest_addr.sin_addr.s_addr,host->h_addr,host->h_length);
        printf("PING %s (%s): %d bytes data in ICMP packets.\n",host->h_name,inet_ntoa(dest_addr.sin_addr),datalen);
    }else{
        memcpy((char*)&dest_addr.sin_addr.s_addr,(char*)&inaddr,sizeof(inaddr));
        printf("PING %s (%s): %d bytes data in ICMP packets.\n",argv[1],inet_ntoa(dest_addr.sin_addr),datalen);
    }
    pid=getpid();
    signal(SIGINT,statistics);
    while(1){
        send_packet();
        recv_packet();
        sleep(1);
    }
    return 0;
}
