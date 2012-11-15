#include <stdio.h>

#define JINZHI		-2
int int2num(int n){
	if(n==0)
		return 0;
	int yu=n%JINZHI;
	if(yu<0)
		yu=yu-JINZHI;
	int2num((n-yu)/JINZHI);
	printf("%d",yu);
	return 0;
}
int main(){
	int n;
	while(1){
		if(scanf("%d",&n)==EOF)
			break;
		if(n==0){
			printf("0\n");
			continue;
		}
		int2num(n);
		printf("\n");
	}
	return 0;
}
