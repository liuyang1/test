#include <linux/module.h>

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/sunddi.h>

MODULE_LICENSE("Dual BSD/GPL");

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

static int hello_init(void){
	printk(KERN_ALERT"Hello,world!\n");
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT"GoodBye,kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);
