// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 server src
#include "np.h"

void *handleclient(void* data);

int main(int argc,char **argv){
	int sockfd,new_fd;
	struct sockaddr_in6 my_addr,their_addr;
	bzero(&my_addr,sizeof(struct sockaddr_in6));
	bzero(&their_addr,sizeof(struct sockaddr_in6));

	unsigned int myport=SERVERPORT;
	unsigned int lisnum=2;
	char buf[MAXBUF+1];

	sockfd=socket(AF_INET6,PROTOCOL,0);
	GUARD(sockfd);
	my_addr.sin6_family=PF_INET6;
	my_addr.sin6_port=htons(myport);
	my_addr.sin6_addr=in6addr_any;
	GUARD(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr_in6)));
	GUARD(listen(sockfd,lisnum));

	socklen_t len;
	while(1){
		len = sizeof(struct sockaddr);
		GUARD(new_fd=accept(
					sockfd,(struct sockaddr *)&their_addr,&len));
		printf("[ACCPT] connect from %s,port %d,socket %d\n",
					inet_ntop(AF_INET6,&their_addr.sin6_addr,buf,sizeof(buf)),
					their_addr.sin6_port,new_fd);

		pthread_t child_thread;
		if(pthread_create(&child_thread,NULL,handleclient,(void*)new_fd)<0){
			perror("create thread");
			continue;
		}
	}
	close(sockfd);
	return 0;
}

void *handleclient(void* data){
	char buf[MAXBUF+1];
	bzero(buf,MAXBUF+1);
	int new_fd=(int)data;

	strcpy(buf,"welcom to np bar!\n");
	int len=send(new_fd,buf,strlen(buf),0);
	GUARD(len);

	while(1){
		bzero(buf,MAXBUF+1);
		len=recv(new_fd,buf,MAXBUF,0);
		if(len<0){
			printf("msg %s failure!\n[error]code:%d info %s\n",
					buf,errno,strerror(errno));
			continue;
		}else{
			if(len!=0)
				printf("[%5d] %s",new_fd,buf);
		}
	}
}
