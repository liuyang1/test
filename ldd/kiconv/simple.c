#include <sys/types.h>
#include <sys/errno.h>
#include <sys/sunddi.h>

int doconversion(char *fromcode,char *tocode,char *inbuf,char *outbuf,
		size_t inlen,size_t *outlen)
{
	kiconv_t cd;
	size_t ileft,ret;
	int err;

	cd = kiconv_open((const char *)tocode,(const char *)fromcode);
	if(cd==(kiconv_t)-1){
		return -1;
	}
	ret= kiconv(cd,&inbuf,&inlen,&outbuf,outlen,&err);
	if(ret==(size_t)-1){
		goto doconv_error_return;
	}
	(void)kiconv_close(cd);
	return 0;
doconv_error_return:
	(void)kiconv_close(cd);
	// need more output buffer
	if(err==E2BIG)
		return -2;
	// illegal sequence
	if(err==EILSEQ)
		return -3;
	// incomplete charcter
	if(err==EINVAL)
		return -4;
	return -5;
}
