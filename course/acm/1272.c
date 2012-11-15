#include <stdio.h>
#include <stdlib.h>

int compare(const void *n1,const void *n2){
	return (*(int*)n1 > *(int*)n2) ? -1 : 1;
}
long long calc(int *p,int n){
	qsort(p,n,sizeof(int),compare);
	int factor;
	int i;
	long long ret=0;
	for(i=0,factor=n-1;i!=n/2;i++,factor-=2){
		ret+=factor*(p[i]-p[n-1-i]);
	}
	return ret;
}
int main(){
	int N,i,j;
	scanf("%d",&N);
	int n,*p,*pf;
	for(i=0;i!=N;i++){
		scanf("%d",&n);
		p=(int*)malloc(sizeof(int)*n);
		for(pf=p;pf!=p+n;pf++)
			scanf("%d",pf);
		//for(pf=p;pf!=p+n;pf++)
		//	printf("%d\n",*pf);
		printf("%lld\n",calc(p,n));
		//for(pf=p;pf!=p+n;pf++)
		//	printf("%d\n",*pf);
		free(p);
	}
	return 0;
}
