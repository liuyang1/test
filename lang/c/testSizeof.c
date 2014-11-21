#include <stdio.h>

int main(){
	printf("sizeof(int):\t%ld\n",sizeof(int));
	printf("sizeof(short):\t%ld\n",sizeof(short));
	printf("sizeof(long):\t%ld\n",sizeof(long));
	printf("sizeof(long long):\t%ld\n",sizeof(long long));
	printf("sizeof(char):\t%ld\n",sizeof(char));

	printf("sizeof(float):\t%ld\n",sizeof(float));
	printf("sizeof(double):\t%ld\n",sizeof(double));
	
	return 0;
}
