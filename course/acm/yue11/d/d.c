#include <stdio.h>

long long biFib(int n){
	if(n==1||n==2)
		return 1;
	if(n==0)
		return 0;
	int f=n&0x01;
	long long fk0,fk1;
	n=n>>1;
	// odd
	// f(2k+1)=f(k+1)^2+f(k)^2;
	if(f==1){
		fk0=biFib(n);
		fk1=biFib(n+1);
		return fk0*fk0+fk1*fk1;
	}
	// even
	// f(2k)=f(k-1)*f(k)+f(k)*f(k+1)
	//      =f(k-1)*f(k)*2+f(k)^2
	else{
		fk0=biFib(n);
		fk1=biFib(n-1);
		return fk1*fk0*2+fk0*fk0;
	}
}

long long biGCD(long long n,long long m){
	if(n<m)
		return biGCD(m,n);
	if(m==0)
		return n;
	else if(m==1)
		return 1;
	int nf=n&0x01;
	int mf=m&0x01;
	if(nf==0&&mf==0)
		return 2*biGCD(n>>1,m>>1);
	if(nf==0&&mf==1)
		return biGCD(n>>1,m);
	if(nf==1&&mf==0)
		return biGCD(n,m>>1);
	return biGCD((n-m)>>1,m);
}

long long first5(long long n){
	while(n>100000)
		n=n/10;
	return n;
}
int main(){
	long long N;
	scanf("%lld",&N);
	long long i;
	long long n,m;
	unsigned long long ret;
	for(i=0;i<N;i++){
		scanf("%lld%lld",&n,&m);
		ret=biFib(biGCD(n,m));
		printf("%lld ",ret);
		printf("%lld ",first5(ret));
		if(ret/(long long)100000000==0){
			printf("%lld\n",ret);
		}else{
			printf("%08lld\n",ret%(long long)100000000);
		}
	}
return 0;
}
