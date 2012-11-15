#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(){
	struct timeval t;
	struct timezone tz;
	gettimeofday(&t,&tz);
	time_t now;
	time(&now);
	printf("time : %ld sec\n",now);
	printf("timeodday : %ld . %ld sec\n",t.tv_sec,t.tv_usec);
	printf("timezone : %ld min\n",tz.tz_minuteswest);
	return 0;
}
