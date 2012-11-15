
// mtime: 2012-10-29 16:27:44
// auth: liuyang
//
// catch segfault event signal SIGSEGV

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void segfault_sigaction_func(int signal,siginfo_t *si,void *arg)
{
	printf("Catch SegFault @ 0x%p \n",si->si_addr);
	printf("Yelp! i can catch SegFault!\n");
//	exit(0);
	return;
}

int main()
{
	struct sigaction sa;
	memset(&sa,0,sizeof(sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction=segfault_sigaction_func;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV,&sa,NULL);

	//int *foo=NULL;
	//*foo=1;// this will cause SegFault
	while(1)
		;

	return 0;
}
