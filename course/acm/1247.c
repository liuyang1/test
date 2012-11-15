#include <stdio.h>
#include <stdlib.h>

int maxKseq(unsigned int* nums,int n,unsigned int s){
	unsigned int st;
	unsinged int i;
	int k0=n/2;
	int s0=0;
	for(i=0;i<k0;i++){
		s0+=nums[i];
	}
	int k;
	for(k
	return 0;
}
int main(){
	int n,i,ret;
	unsigned int s;
	unsigned int * nums,*num;
	while(scanf("%d%d\n",&n,&s)!=EOF){
		nums=(unsigned int *)malloc(sizeof(unsigned int)*n);
		for(num=nums;num!=nums+n;num++){
			scanf("%d",num);
		}
		printf("input nums:\n");
		for(num=nums;num!=nums+n;num++){
			printf("%d ",*num);
		}
		printf("\n");
		ret=maxKseq(nums,n,s);
		printf("ret:%d\n",ret);
		free(nums);
	}
	return 0;
}
