#include <stdio.h>
#define PRIME	1000000007
const unsigned int power[]={1,1000000007u ,3923446833u ,3709206359u ,241869153u ,4154127271u ,418672529u ,384923383u ,2786010817u};
unsigned int hash(const char *str){
	unsigned int res=0;
	for(;*str;str++){
		res*=1000000007;// !!PRIME!!
		res+=*str;
	}
	return res;
}

int main(){
	int i;
	int res=PRIME;
	for(i=0;i<8;i++){
		printf(",%uu ",res);
		res*=PRIME;
	}
	return 0;
}
