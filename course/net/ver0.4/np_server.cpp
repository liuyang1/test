// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 server src
// liuyang1,2011-11-25 21:55:24
// libevent support
// liuyang1,2011-11-26 20:58:46
// libevent support OK!
// liuyang1,2011-11-26 20:59:15
// ncurses,window sperate support
#include "np.h"
#include <list>

using namespace std;

list<struct Client*>		clientList;

// 广播消息
int broadcastMsg(char *str){
	struct evbuffer *evbuf=evbuffer_new();
	evbuffer_add_printf(evbuf,"board: %s",str);
	list<struct Client*>::iterator  iter;
	for(iter=clientList.begin();iter!=clientList.end();++iter){
		printf("broadcast to %d %s %p\n",(*iter)->sockfd,str,(*iter)->buf_ev);
		//bufferevent_write_buffer((*iter)->buf_ev,evbuf);
		//直接发送广播
		send((*iter)->sockfd,str,strlen(str),0);
	}
	printf("boardcast over\n");
	evbuffer_free(evbuf);
	return 0;
}

// 接收消息
void buf_read_callback(struct bufferevent *in,void *arg){
#ifdef _TEST
	printf("buf read\n");
#endif
	char req[MAXBUF+1];
	bufferevent_read(in,&req,MAXBUF);
	char *p=req;
	while(*p!='\n'&&*p!='\r'){
		p++;
	}
	*p='\0';
	//req=evbuffer_readline(in->input);
	broadcastMsg(req);
//	free(req);
}

void buf_write_callback(struct bufferevent *bev,void *arg){
	struct Client *client=(struct Client*)arg;
	char *input=evbuffer_readline(client->buf_ev->input);
	printf("write %d %s\n",client->sockfd,input);
	free(input);
}

// 错误处理
void buf_error_callback(struct bufferevent *bev,short what,void *arg){
	struct Client *client=(struct Client *)arg;
	printf("[CLOSE] sock %d\n",client->sockfd);
	bufferevent_free(client->buf_ev);
	close(client->sockfd);
	list<struct Client*>::iterator iter;
	for(iter=clientList.begin();iter!=clientList.end();iter++){
		if((*iter)->sockfd==client->sockfd){
			clientList.erase(iter);
			break;
		}
	}
	free(client);
}

// 接受连接请求
void accept_callback(int fd,short ev,void *arg){
#ifdef _TEST
	printf("[%5d]: accept\n",fd);
#endif
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len=sizeof(client_addr);
	struct Client *client=NULL;

	GUARD(client_fd=accept(fd,(struct sockaddr *)&client_addr,&client_len));

	setnonblock(client_fd);
	client=(struct Client*)calloc(1,sizeof(struct Client));
	if(client==NULL){
		error_printf("malloc failed\n");
	}
	char tmp[]="welcome to bar!";
	int len=send(client_fd,tmp,strlen(tmp),0);
	GUARD(len);

	client->sockfd=client_fd;
	client->buf_ev=bufferevent_new(client_fd,
			buf_read_callback,buf_write_callback,buf_error_callback,client);
	bufferevent_enable(client->buf_ev,EV_READ|EV_WRITE);
	clientList.push_back(client);
#ifdef _TEST
	printf("[%5d]: accept over\n",fd);
#endif
}

// 主函数
// 开启socket监听,同时开始事件循环
int main(int argc,char **argv){

	int sockfd;
	struct sockaddr_in6 my_addr,their_addr;
	bzero(&my_addr,sizeof(struct sockaddr_in6));
	bzero(&their_addr,sizeof(struct sockaddr_in6));

	unsigned int myport=SERVERPORT;
	unsigned int lisnum=10;

	sockfd=socket(AF_INET6,PROTOCOL,0);
	GUARD(sockfd);
	my_addr.sin6_family=PF_INET6;
	my_addr.sin6_port=htons(myport);
	my_addr.sin6_addr=in6addr_any;
	GUARD(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr_in6)));
	GUARD(listen(sockfd,lisnum));

	struct event accept_event;
	int reuse=1;
	event_init();

	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	setnonblock(sockfd);

	event_set(&accept_event,sockfd,EV_READ|EV_PERSIST,accept_callback,NULL);
	event_add(&accept_event,NULL);

	event_dispatch();

	close(sockfd);

	return 0;
}
