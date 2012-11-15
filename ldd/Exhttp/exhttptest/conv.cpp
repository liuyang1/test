#include <iconv.h>
#include <string.h>
#include <chardet.h>
#include "conv.h"
#include <stdio.h>

int ConvEx(char *inbuf,size_t inlen,char *outbuf,size_t outlen){
	
	char encode[20]="utf-8";
	int ret;
//	ret=get_encoding(inbuf,inlen,encode);
//	if(ret!=0)
//		return -1;
	
	printf("\n\nencode: %s\n",encode);
	//code_convert(encode,"utf-8",inbuf,inlen,outbuf,outlen);
	code_convert(encode,"gb2312",inbuf,inlen,outbuf,outlen);
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
