#include "np.h"

void error_printf(char *str){
#ifdef _TEST
	printf("[ERROR] %s\n",str);
#endif
}
void test_printf(char *str){
#ifdef _TEST
	printf("[SUCC ] %s\n",str);
#endif
}

void CharMsg2msg(struct ChatMsg *src,char * msg){
	sprintf(msg,"%10s%10s%1024s",src->fromUser,src->toUser,src->mBuf);
}

void msg2CharMsg(char *msg,struct ChatMsg *src){
	strcpy(src->fromUser,msg);
	strcpy(src->toUser,msg+10);
	strcpy(src->mBuf,msg+20);
}

//inline void CtrlMsg2msg(struct CtrlMsg *src,char *msg){
//	sprintf(msg,"%c%1024s",msg
//intline void AddUser2Ctrl(

int setnonblock(int fd){
	int flag;
	flag=fcntl(fd,F_GETFL);
	flag|=O_NONBLOCK;
	fcntl(fd,F_SETFL,flag);
	return 0;
}

int initSSL(){
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	return 0;
}
/*
void ShowCerts(SSL *sslfd){
	X509 *cert;
	char *line;
	cert=SSL_get_peer_certificate(ssl);
	if(cert!=NULL){
		printf("
*/			
