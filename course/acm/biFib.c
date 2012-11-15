// fast fibnacci algo
// liuyang1,2011-12-01 14:32:43
// algo come from baidubaike
#include <stdio.h>

long long fib(int n){
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
		fk0=fib(n);
		fk1=fib(n+1);
		return fk0*fk0+fk1*fk1;
	}
	// even
	// f(2k)=f(k-1)*f(k)+f(k)*f(k+1)
	//		=f(k-1)*f(k)*2+f(k)^2
	else{
		fk0=fib(n);
		fk1=fib(n-1);
		return fk1*fk0*2+fk0*fk0;
	}
}

long long f(long long n){
	long long f1=1,f2=1;
	int i=1;
	while(1){
		if(i==n)	return f1;
		if(i+1==n)	return f2;
		f1=f1+f2;
		f2=f1+f2;
		i+=2;
	}
}

int main(){
	int n;
	while(1){
		if(EOF==scanf("%d",&n))
			break;
		printf("%lld\n",f(n));
	}
	return 0;
}
		
