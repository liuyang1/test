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

// 接收服务器消息并显示		线程
void *recvThread(void *data);
WINDOW *recvWin;
WINDOW *inputWin;

// 主函数
// 开启socket,并且接收用户输入,发送到服务器
int main(int argc,char** argv){
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

	// 使用ncurses窗口系统
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
	len=recv(sockfd,buf,MAXBUF,0);
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
		len=send(sockfd,buf1,strlen(buf1),0);
		if(len<0)
			continue;
	}
	close(sockfd);
	delwin(recvWin);
	delwin(inputWin);
	delwin(recvBd);
	delwin(inputBd);
	endwin();
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
