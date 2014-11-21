// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 client src
// liuyang1,2011-11-27 01:54:04
// libncurese support OK!
// basic applciation ok!
// liuyang1,2011-11-27 01:58:31
// support user nick name
// liuyang1,2011-11-27 02:06:13
// support chinese
#include "np.h"
#include <ncurses.h>
#include <locale.h>

#define ESCAPE	27
#define VLINE	'-'
#define HLINE	'|'
#define HEIGHT	1
#define HEIGHT1	10
#define WIDTH	60

void *recvThread(void *data);
WINDOW *recvWin;
WINDOW *inputWin;

SSL_CTX *ctx;
SSL		*sslfd;

int main(int argc,char** argv){
	// openssl support
#ifdef _SSL
	initSSL();
	ctx=SSL_CTX_new(SSLv23_client_method());
	if(ctx==NULL){
		ERR_print_errors_fp(stdout);
		exit(-2);
	}
#endif

	if(argc!=2){
		printf("Usage:client ipv6addr\n");
		exit(-1);
	}
	setlocale(LC_ALL,"");
	char name[10];
	printf("输入昵称:\n");
	scanf("%6s",name);

	int sockfd,len;
	struct sockaddr_in6	dst;
	char buf[MAXBUF+1];
	char buf1[MAXBUF+1];

	sockfd=socket(AF_INET6,PROTOCOL,0);
	GUARD(sockfd);

	bzero(&dst,sizeof(dst));
	dst.sin6_family=AF_INET6;
	dst.sin6_port=htons(SERVERPORT);
	GUARD(inet_pton(AF_INET6,argv[1],&dst.sin6_addr));
	GUARD(connect(sockfd,(struct sockaddr *)&dst,sizeof(dst)));

	// ssl support
#ifdef _SSL
	sslfd=SSL_new(ctx);
	SSL_set_fd(sslfd,sockfd);
	if(SSL_connect(sslfd)==-1)
		ERR_print_errors_fp(stderr);
	else{
		printf("connected with %s encryption\n",SSL_get_cipher(sslfd));
		//ShowCerts(sslfd);
	}
#endif
	// ssl support bind over
	
	initscr();
	WINDOW *recvBd=subwin(stdscr,HEIGHT1+2,WIDTH+2,1,1);
	WINDOW *inputBd=subwin(stdscr,HEIGHT+2,WIDTH+2,15,1);
	recvWin=subwin(stdscr,HEIGHT1,WIDTH,2,2);
	inputWin=subwin(stdscr,HEIGHT,WIDTH,16,2);
	box(recvBd,HLINE,VLINE);
	box(inputBd,HLINE,VLINE);
	wprintw(recvBd,"接收");
	wprintw(inputBd,"发送");
	cbreak();
	keypad(stdscr,TRUE);
	start_color();
	scrollok(recvWin,1);
	scrollok(inputWin,1);
	refresh();

	bzero(buf,MAXBUF+1);
#ifndef _SSL
	len=recv(sockfd,buf,MAXBUF,0);
#else
	len=SSL_read(sslfd,buf,MAXBUF);
#endif
	wprintw(recvWin,"%s\n",buf);
	touchwin(recvWin);
	wrefresh(recvWin);

	pthread_t t_recv;
	if(pthread_create(&t_recv,NULL,recvThread,(void*)sockfd)<0){
		perror("create thread");
		exit(1);
	}

	while(1){
		bzero(buf,MAXBUF+1);
		wprintw(inputWin,"%6s > ",name);
		int key=wgetch(inputWin);
		if(key==ESCAPE){
			break;
		}
		wscanw(inputWin,"%s",buf);
		touchwin(inputWin);
		wrefresh(inputWin);

		sprintf(buf1,"[%6s]: %c%s\n",name,(char)key,buf);
#ifndef _SSL
		len=send(sockfd,buf1,strlen(buf1),0);
#else
		len=SSL_write(sslfd,buf1,strlen(buf1));
#endif
		if(len<0)
			continue;
	}
	close(sockfd);
	delwin(recvWin);
	delwin(inputWin);
	delwin(recvBd);
	delwin(inputBd);
	endwin();
#ifdef _SSL
	SSL_shutdown(sslfd);
	SSL_free(sslfd);
	SSL_CTX_free(ctx);
#endif
	return 0;
}


void *recvThread(void* data){
#ifndef _SSL
	int sockfd=(int)data;
#endif
	char buf[MAXBUF+1];
	int len;
	while(1){
		bzero(buf,MAXBUF+1);
#ifndef _SSL
		len=recv(sockfd,buf,MAXBUF,0);
#else
		len=SSL_read(sslfd,buf,MAXBUF);
#endif
		if(len<0)
			continue;
		else if(len!=0){
			wprintw(recvWin,"%s\n",buf);
			wprintw(inputWin,"");
			touchwin(recvWin);
			touchwin(inputWin);
			wrefresh(recvWin);
			wrefresh(inputWin);
		}
	}
}
