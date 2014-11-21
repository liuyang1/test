// liuyang1,2011-11-22 16:14:10
// network & protocol course experiment
// socket @ ipv6 server src
// liuyang1,2011-11-25 21:55:24
// libevent support
// liuyang1,2011-11-26 20:58:46
// libevent support OK!
// liuyang1,2011-11-26 20:59:15
// ncurses,window sperate support
// liuyang1,2011-11-27 21:44:14
// openssl support
// liuyang1,2011-11-27 23:01:06
// openssl support client over
// openssl libevent must combined to handle
// so server TODO
// liuyang1,2011-11-28 00:57:50
// openssl@libevent support until libevent 2.0.3
// should learn from libevent/sample/le-proxy.c this code
#include "np.h"
#include <list>

/* define HOME to be dir for key and cert files... */
#define HOME "./"
/* Make these what you want for cert & key files */
#define CERTF  HOME "foo-cert.pem"
#define KEYF  HOME  "foo-cert.pem"

using namespace std;

#ifdef _SSL
SSL_CTX *ctx=NULL;
#endif

list<struct Client*>		clientList;

int broadcastMsg(char *str){
	struct evbuffer *evbuf=evbuffer_new();
	evbuffer_add_printf(evbuf,"board: %s",str);
	list<struct Client*>::iterator  iter;
	int len;
	for(iter=clientList.begin();iter!=clientList.end();++iter){
		printf("broadcast to %d %s %p\n",(*iter)->sockfd,str,(*iter)->buf_ev);
		//bufferevent_write_buffer((*iter)->buf_ev,evbuf);
		// normal send
		//send((*iter)->sockfd,str,strlen(str),0);
		// ssl send
		// package is plain text?!
		len=SSL_write((*iter)->sslfd,str,strlen(str));
		GUARD(len);
	}
	printf("boardcast over\n");
	evbuffer_free(evbuf);
	return 0;
}

void buf_read_callback(struct bufferevent *in,void *arg){
#ifdef _TEST
	printf("buf read\n");
#endif
	//char *req;
	//req=evbuffer_readline(in->input);
	
	//struct Client *client=(struct Client*)arg;

	char req[MAXBUF+1];
	req[0]='\0';
	//SSL_read(client->sslfd,req,MAXBUF);
	//bufferevent_read(in,&req,MAXBUF);
	char *p;
	for(p=req;*p!='\r'&&*p!='\n';p++)
		;
	*p='\0';
	broadcastMsg(req);
}

void ssl_readcb(struct bufferevent *bev,void *arg){
	struct evbuffer *in=bufferevent_get_input(bev);
	printf("received %zu bytes\n",evbuffer_get_length(in));
	printf("%.*s\n",(int)evbuffer_get_length(in),evbuffer_pullup(in,-1));

	bufferevent_write_buffer(bev,in);
}

void buf_write_callback(struct bufferevent *bev,void *arg){
	struct Client *client=(struct Client*)arg;
	char *input=evbuffer_readline(client->buf_ev->input);
	printf("write %d %s\n",client->sockfd,input);
	free(input);
}

void buf_error_callback(struct bufferevent *bev,short what,void *arg){
	struct Client *client=(struct Client *)arg;
	bufferevent_free(client->buf_ev);
	close(client->sockfd);
	SSL_shutdown(client->sslfd);
	SSL_free(client->sslfd);
	list<struct Client*>::iterator iter;
	for(iter=clientList.begin();iter!=clientList.end();iter++){
		if((*iter)->sockfd==client->sockfd){
			clientList.erase(iter);
			break;
		}
	}
	free(client);
}

void accept_callback(int fd,short ev,void *arg){
#ifdef _TEST
	printf("[%5d]: accept\n",fd);
#endif
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len=sizeof(client_addr);
	struct Client *client=NULL;

	GUARD(client_fd=accept(fd,(struct sockaddr *)&client_addr,&client_len));

	SSL *ssl=SSL_new(ctx);
	SSL_set_fd(ssl,client_fd);
	if(SSL_accept(ssl)==-1){
		perror("accept");
		close(client_fd);
		return;
	}

	setnonblock(client_fd);
	client=(struct Client*)calloc(1,sizeof(struct Client));
	if(client==NULL){
		error_printf("malloc failed\n");
	}
	char tmp[]="welcome to bar!";
	//int len=send(client_fd,tmp,strlen(tmp),0);
	int len=SSL_write(ssl,tmp,strlen(tmp));
	GUARD(len);

	client->sockfd=client_fd;
#ifndef _SSL
	client->buf_ev=bufferevent_new(client_fd,
			buf_read_callback,buf_write_callback,buf_error_callback,client);
	bufferevent_enable(client->buf_ev,EV_READ|EV_WRITE);
	clientList.push_back(client);
#else
	client->sslfd=ssl;
//	client->buf_ev=bufferevent_openssl_socket_new(base,

	client->buf_ev=bufferevent_new(client_fd,
			buf_read_callback,buf_write_callback,buf_error_callback,client);
	bufferevent_enable(client->buf_ev,EV_READ|EV_WRITE);
	clientList.push_back(client);
#endif	
#ifdef _TEST
	printf("[%5d]: accept over\n",fd);
#endif
}

static void ssl_acceptcb(struct evconnlistener *serv,int sock,struct sockaddr*sa,int sa_len,void *arg){
	printf("new ssl accept\n");
	struct event_base	*evbase;
	struct bufferevent	*bev;
	SSL_CTX				*server_ctx;
	SSL					*client_ctx;
	server_ctx	=	(SSL_CTX *)arg;
	client_ctx	=	SSL_new(server_ctx);
	evbase	=	evconnlistener_get_base(serv);
	bev		=	bufferevent_openssl_socket_new(evbase,sock,client_ctx,
			BUFFEREVENT_SSL_ACCEPTING,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_enable(bev,EV_READ|EV_WRITE);
	bufferevent_setcb(bev,ssl_readcb,NULL,NULL,NULL);
}

int main(int argc,char **argv){
	//ssl support
	initSSL();
	ctx=SSL_CTX_new(SSLv23_server_method());
	if(ctx==NULL){
		ERR_print_errors_fp(stdout);
		exit(-2);
	}
	if(SSL_CTX_use_certificate_file(ctx,"cacert.server.pem",SSL_FILETYPE_PEM)<=0){
		ERR_print_errors_fp(stdout);
		exit(-2);
	}
	if(SSL_CTX_use_PrivateKey_file(ctx,"privkey.server.pem",SSL_FILETYPE_PEM)<=0){
		ERR_print_errors_fp(stdout);
		exit(-2);
	}
	//ssl support init over

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

	// libevnet support
#ifndef _SSL
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
#else
	struct evconnlistener	*listener;
	struct event_base		*evbase=event_base_new();
	listener = evconnlistener_new_bind(
			evbase,ssl_acceptcb,(void*)ctx,
			LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,1024,
			(struct sockaddr *)&my_addr,sizeof(struct sockaddr_in6));
	event_base_loop(evbase,0);
	
	evconnlistener_free(listener);
#endif
	// libevent support over


	close(sockfd);
#ifdef SSL
	SSL_CTX_free(ctx);
#endif
	return 0;
}
