/* acm@ustc 1215
 * liuyang
 */
// 现在算法的复杂度为O(M*N),会出现超时问题
// 寻找更快得计算方案
#include <stdio.h>
#include <stdlib.h>

#define PLANT(p,n,i)	*(p+i%n)
//一种类似于贪心的方法应该可以在O(N)解决这个问题
int plat(int n,int k,int* ip){
	int max,min;
	int i,j;
	int ret=0;
	int mmax=0;
	int tmp;
	for(i=0;i<n;i++){
		max=ip[i];
		min=ip[i];
		mmax=0;
		for(j=1;j<=n-1;j++){
			tmp=PLANT(ip,n,i+j);
			if(tmp>max)
				max=tmp;
			else if(tmp<min)
				min=tmp;
			if(max-min>k){
				ret=j;
				break;
			}
			if(j==n-1)
				return n;
		}
		if(ret>mmax)
			mmax=ret;
	}
	return mmax+1;
}

int main(){
	int n,k;
	int *ip,*p;
	int ret;
	while(1){
		scanf("%d%d",&n,&k);
		if(n==0&&k==0)
			return 0;
		ip=(int*)malloc(sizeof(int)*n);
		for(p=ip;p!=ip+n;p++)
			scanf("%d",p);

		ret=plat(n,k,ip);
		printf("%d\n",ret);
		free(ip);
	}
	return 0;
}
