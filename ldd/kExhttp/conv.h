#ifndef _CONV_H_
#define _CONV_H_

#include <linux/string.h>
// convert external interface
int ConvEx(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

// get input string encoding
int get_encoding(char *inbuf,size_t inlen,char *encode);

// chartset convert
int code_convert(char *from_charset,char *to_charset,
		char* inbuf,size_t inlen,
		char* outbuf,size_t outlen);

// from utf-8 to gb2312
int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
// from gb2312 to utf-8
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

int fib(int n);
#endif
