#include <stdio.h>

int biGCD(int n,int m){
	if(n<m)
		return biGCD(m,n);
	if(m==0)
		return n;
	else if(m==1)
		return 1;
	int f1=n&0x01;
	int f2=n&0x01;
	if(f1==0){
		if(f2==0)
			return 2*biGCD(n>>1,m>>1);
		else
			return biGCD(n>>1,m);
	}else{
		if(f2==0)
			return biGCD(n,m>>1);
		else
			return biGCD((n-m)>>1,m);
	}
}

int main(){
	int N=10000,i;
	int n,m;
	for(n=1;n<N;n++){
		for(m=1;m<N;m++){
			biGCD(n,m);
		}
		if(n%1000==999)
			printf("%d\n",n);
	}
    return 0;
}

