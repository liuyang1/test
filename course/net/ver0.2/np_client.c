// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 client src
#include "np.h"

// 接收服务器消息并显示		线程
void *recvThread(void *data);

// 主函数
// 开启socket,并且接收用户输入,发送到服务器
int main(int argc,char** argv){
	int sockfd,len;
	struct sockaddr_in6	dst;
	char buf[MAXBUF+1];

	if((sockfd=socket(AF_INET6,PROTOCOL,0))<0){
		perror("socket created error\n");
		exit(errno);
	}
	printf("socked created error\n");

	bzero(&dst,sizeof(dst));
	dst.sin6_family=AF_INET6;
	dst.sin6_port=htons(SERVERPORT);
	if(inet_pton(AF_INET6,argv[1],&dst.sin6_addr)<0){
		perror(argv[1]);
		exit(errno);
	}
	printf("addr created\n");

	if(connect(sockfd,(struct sockaddr *)&dst,sizeof(dst))!=0){
		perror("connect");
		exit(errno);
	}
	printf("server connected\n");

	bzero(buf,MAXBUF+1);
	len=recv(sockfd,buf,MAXBUF,0);
	if(len>0){
		printf("recv msg:%s ,%dBytes\n",buf,len);
	}else{
		printf("recv msg error,code: %d,info: %s\n",errno,strerror(errno));
	}

	pthread_t t_recv;
	if(pthread_create(&t_recv,NULL,recvThread,(void*)sockfd)<0){
		perror("create thread");
		exit(1);
	}

	while(1){
		bzero(buf,MAXBUF+1);
		scanf("%s",buf);
		len=send(sockfd,buf,strlen(buf),0);
		if(len<0){
			printf("send msg error,code: %d,info: %s\n",errno,strerror(errno));
		}else{
			//printf("send msg:%s, %dBytes\n",buf,len);
		}
		chkbye(buf);
	}
	close(sockfd);
	return 0;
}


void *recvThread(void* data){
	int sockfd=(int)data;
	char buf[MAXBUF+1];
	int len;
	while(1){
		bzero(buf,MAXBUF+1);
		len=recv(sockfd,buf,MAXBUF,0);
		printf("%s\n",buf);
		chkbye(buf);
	}
}
