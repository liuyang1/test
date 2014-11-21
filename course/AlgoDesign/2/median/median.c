#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void swap(int *n1,int *n2){
	int t=*n1;
	*n1=*n2;
	*n2=t;
}

int irand(int b,int e){
	return rand()%(e-b+1)+b;
}

int Partition(int *A,int p,int r){
	int x=A[r];
	int i=p-1,j;
	for(j=p;j<=r-1;j++){
		if(A[j]<=x){
			i++;
			swap(A+i,A+j);
		}
	}
	swap(A+i+1,A+r);
	return i+1;
}

int RandPartition(int *A,int p,int r){
	int i=irand(p,r);
	swap(A+r,A+i);
	return Partition(A,p,r);
}

int RandSelect(int *A,int p,int r,int i){
	if(p==r)
		return A[p];
	int q=RandPartition(A,p,r);
	int k=q-p+1;
	if(i==k)
		return A[q];
	else if(i<k)
		return RandSelect(A,p,q-1,i);
	else
		return RandSelect(A,q+1,r,i-k);
}

int lowerMedian(int *A,int n){
	return RandSelect(A,0,n-1,(n+1)/2);
}

int upperMedian(int *A,int n){
	return RandSelect(A,0,n-1,(n+2)/2);
}

int main(){
	srand((unsigned int)time(NULL));
	int N,i,x,sum;
	scanf("%d",&N);
	int *A=(int*)malloc(sizeof(int)*N);
	for(i=0;i<N;i++)
		scanf("%d%d",&x,A+i);
	int ret1=lowerMedian(A,N);
	//int ret2=upperMedian(A,N);
	sum=0;
	for(i=0;i<N;i++)
		sum+=A[i]>ret1?A[i]-ret1:ret1-A[i];
	printf("%d\n",sum);
	free(A);
	return 0;
}
