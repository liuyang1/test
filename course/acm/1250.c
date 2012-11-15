#include <stdio.h>

int cnt5(int n){
	if(n<5)
		return 0;
	return n/5+cnt5(n/5);
}
int main(){
	int N,i;
	scanf("%d",&N);
	int n;
	for(i=0;i<N;i++){
		scanf("%d",&n);
		printf("%d\n",cnt5(n));
	}
	return 0;
}

