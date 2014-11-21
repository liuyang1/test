#include <linux/module.h>
#include "conv.h"
#include "exhttp.h"

MODULE_LICENSE("Dual BSD/GPL");

static int test(void){
	return fib(10);
}

static int tconv_init(void){
	printk(KERN_ALERT"insmod tconv\n");
	printk(KERN_ALERT"fib: %d\n",test());
	return 0;
}

static void tconv_exit(void){
	printk(KERN_ALERT"rmmod tconv\n");
}


module_init(tconv_init);
module_exit(tconv_exit);
