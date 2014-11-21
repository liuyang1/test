// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 client src
#include "np.h"

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

	bzero(buf,MAXBUF+1);
	strcpy(buf,"what a fuck day!");
	len=send(sockfd,buf,strlen(buf),0);
	if(len<0){
		printf("send msg:%s, %dBytes\n",buf,len);
	}else{
		printf("sned msg error,code: %d,info: %s\n",errno,strerror(errno));
	}
	close(sockfd);
	return 0;
}
