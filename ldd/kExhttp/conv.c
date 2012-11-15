// using libarry:
// iconv,c std,chartdet
#include <linux/string.h>
#include "conv.h"

// how to port this library to kernel???
// after port just define CONV marcos to use iconv and chardet code
#ifdef CONV
#include <kiconv.h>
#include <chardet.h>
#endif

#ifdef CONV
int ConvEx(char *inbuf,size_t inlen,char *outbuf,size_t outlen){
	
	char encode[20];
	int ret=get_encoding(inbuf,inlen,encode);
	if(ret!=0)
		return -1;
	code_convert(encode,"utf-8",inbuf,inlen,outbuf,outlen);
	return 0;
}

int get_encoding(char *inbuf,size_t inlen,char *encode)
{
	chardet_t chardet=NULL;
	if(chardet_create(&chardet)!=CHARDET_RESULT_OK)
		return -1;
	if(chardet_handle_data(chardet,inbuf,inlen)!=CHARDET_RESULT_OK)
		return -1;
	if(chardet_data_end(chardet)==CHARDET_RESULT_OK){
		chardet_get_charset(chardet,encode,CHARDET_MAX_ENCODING_NAME);
	}
	if(chardet)
		chardet_destroy(chardet);
	return 0;
}


int code_convert(char *from_charset,char *to_charset,
		char* inbuf,size_t inlen,
		char* outbuf,size_t outlen)
{
	iconv_t cd;
	//int rc;
	char **pin=&inbuf;
	char **pout=&outbuf;

	cd=iconv_open(to_charset,from_charset);
	if(cd==0)
		return -1;
	memset(outbuf,0,outlen);
	size_t ret=iconv(cd,pin,&inlen,pout,&outlen);
	if(ret==(size_t)-1)
		return -1;
	iconv_close(cd);
	return 0;
}

int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
#endif
int fib(int n){
	if(n==1||n==2)
		return 1;
	return fib(n-1)+fib(n-2);
}
