// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 server src
#include "np.h"

int main(int argc,char **argv){
	int sockfd,new_fd;
	socklen_t	len=sizeof(struct sockaddr);

	struct sockaddr_in6 my_addr,their_addr;
	bzero(&my_addr,sizeof(struct sockaddr_in6));
	bzero(&their_addr,sizeof(struct sockaddr_in6));

	unsigned int myport=SERVERPORT;
	unsigned int lisnum=2;
	char buf[MAXBUF+1];

	if((sockfd=socket(AF_INET6,PROTOCOL,0))<0){
		perror("socket");
		exit(1);
	}else{
		printf("socket created\n");
	}

	my_addr.sin6_family=PF_INET6;
	my_addr.sin6_port=htons(myport);
	my_addr.sin6_addr=in6addr_any;
	//printf("ipv6 addr %s\n",argv[1]);
	//inet_pton(AF_INET6,argv[1],&my_addr.sin6_addr);

	if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr_in6))==-1){
		perror("bind error\n");
		exit(1);
	}else{
		printf("binded inet6 %d port ip6addrTODO\n",myport);
	}

	if(listen(sockfd,lisnum)==-1){
		perror("listen");
		exit(1);
	}else{
		printf("listen %d\n",lisnum);
	}

	while(1){
		len = sizeof(struct sockaddr);
		if((new_fd=accept(sockfd,(struct sockaddr *)&their_addr,&len))==-1){
			perror("accept error\n");
			exit(errno);
		}else{
			printf("[accept] connect from %s,port %d,socket %d\n",
					inet_ntop(AF_INET6,&their_addr.sin6_addr,buf,sizeof(buf)),
					their_addr.sin6_port,new_fd);
		}

		bzero(buf,MAXBUF+1);
		strcpy(buf,"server init to got client\n");
		len = send(new_fd,buf,strlen(buf),0);
		if(len<0){
			printf("msg %s failure!\n[error]code:%d info %s\n",
					buf,errno,strerror(errno));
		}else{
			printf("msg %s successed! %d bytes\n",buf,len);
		}

		bzero(buf,MAXBUF+1);
		len=recv(new_fd,buf,MAXBUF,0);
		if(len<0){
			printf("msg %s failure!\n[error]code:%d info %s\n",
					buf,errno,strerror(errno));
		}else{
			printf("msg %s successed! %d bytes\n",buf,len);
		}

	}
	close(sockfd);
	return 0;
}
