#include <stdio.h>

void calc(double m,double n,int *k,int *N){
	int i=1;
	*k=i;
	*N=(int)(n*i);
}
int main(){
	int N;
	scanf("%d",&N);
	int i;
	double m,n;
	int k,Val;
	for(i=0;i!=N;i++){
		scanf("%lf%lf",&m,&n);
		calc(m,n,&k,&Val);
		printf("%d\n%d\n",Val,k);
	}
	return 0;
}
