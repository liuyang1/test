// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 client src
#include "np.h"

void *recvThread(void *data);

int main(int argc,char** argv){
	if(argc!=2){
		printf("Usage:client ipv6addr\n");
		exit(-1);
	}
	int sockfd,len;
	struct sockaddr_in6	dst;
	char buf[MAXBUF+1];

	sockfd=socket(AF_INET6,PROTOCOL,0);
	GUARD(sockfd);

	bzero(&dst,sizeof(dst));
	dst.sin6_family=AF_INET6;
	dst.sin6_port=htons(SERVERPORT);
	GUARD(inet_pton(AF_INET6,argv[1],&dst.sin6_addr));
	GUARD(connect(sockfd,(struct sockaddr *)&dst,sizeof(dst)));

	bzero(buf,MAXBUF+1);
	len=recv(sockfd,buf,MAXBUF,0);
	printf("[AD   ] %s",buf);

	pthread_t t_recv;
	if(pthread_create(&t_recv,NULL,recvThread,(void*)sockfd)<0){
		perror("create thread");
		exit(1);
	}

	while(1){
		bzero(buf,MAXBUF+1);
		//scanf("%s",buf);
		fgets(buf,MAXBUF,stdin);
		len=send(sockfd,buf,strlen(buf),0);
		if(len<0)
			continue;
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
		if(len<0)
			continue;
		else if(len!=0)
			printf("%s\n",buf);
	}
}
