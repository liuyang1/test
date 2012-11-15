#include <stdio.h>
#include <unistd.h>

int main(){
	int i;
	for(i=0;i<100;i++){
		printf("\rfinished %3d\r",i);
		//fflush(stdout);
		sleep(1);
	}
	return 0;
}
