#ifndef _CONV_H_
#define _CONV_H_

#include <string.h>

int ConvEx(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

int get_encoding(char *inbuf,size_t inlen,char *encode);

int code_convert(char *from_charset,char *to_charset,
		char* inbuf,size_t inlen,
		char* outbuf,size_t outlen);

int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

#endif
