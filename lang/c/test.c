#include <stdio.h>

int test(){
	unsigned int len=3;
	unsigned int i;
	for(i=0;i<len-4;i++){
		printf("loop %u %u\n",len-4,i);
	}
	return 0;
}

int testUnsigned(){
	printf("%d\n",1-2);
	printf("%u\n",1-2);
	printf("%u\n",(unsigned int)1-(unsigned int)2);
	printf("%u\n",(unsigned int)(1-2));
	printf("%u\n",(int)((unsigned int)(1-2)));
	printf("%d\n",(int)((unsigned int)(1-2)));
	return 0;
}

int testPrint(){
	long long i=1000;
	printf("%llu %llu\n",i,i);
	return 0;
}
int main(){
	return testPrint();
}
