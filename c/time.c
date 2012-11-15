#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	time_t now;
	struct tm *timenow;
	time(&now);
	printf("%d\n",now);
	timenow=localtime(&now);
	printf("local time is %d\t%s\n",now,asctime(timenow));
	return 0;
}
