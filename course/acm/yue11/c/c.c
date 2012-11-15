#include <stdio.h>
#include <stdlib.h>

int chkp1(int* p,int* p1,int n){
	int i;
	for(i=0;i<n;i++){
		p1[i]=(p[i]==i+1);
	}
	return 0;
}
int chkp2(int* p,int* p2,int n){
	int i;
	for(i=1;i<n;i++){
		p2[i]=(p[i]==i);
	}
	return 0;
}
int cum(int *p1,int n){
	int i;
	for(i=1;i<n;i++){
		p1[i]=p1[i]+p1[i-1];
	}
	return 0;
}
int icum(int *p2,int n){
	int i;
	for(i=n-2;i>0;i--){
		p2[i]=p2[i]+p2[i+1];
	}
	return 0;
}
int sum(int *p1,int *p2,int n){
	int i;
	for(i=0;i<n;i++){
		p1[i]=p1[i]+p2[i];
	}
	return 0;
}
int chksum(int *p1,int n){
	int max=p1[0],i;
	int index=0;
	for(i=0;i<n;i++){
		if(p1[i]>=max){
			max=p1[i];
			index=i;
		}
	}
	return index;
}
int del(int *p,int index,int n){
	int i;
	for(i=index;i<n-1;i++){
		p[i]=p[i+1];
	}
	return 0;
}
int disp(int* p,int n){
	/*
	int i;
	for(i=0;i<n;i++){
		printf("%d ",p[i]);
	}
	printf("\n");
	*/
	return 0;
}
int main(){
	int N;
	scanf("%d",&N);
	int i,j;
	int n,*p,*p1,*p2;
	int ret;
	for(i=0;i<N;i++){
		scanf("%d",&n);
		p=(int*)malloc(n*sizeof(int));
		p1=(int*)malloc(n*sizeof(int));
		p2=(int*)malloc(n*sizeof(int));
		for(j=0;j<n;j++)
			scanf("%d",p+j);
		while(1){
			chkp1(p,p1,n);
			disp(p1,n);
			chkp2(p,p2,n);
			disp(p2,n);
			cum(p1,n);
			disp(p1,n);
			icum(p2,n);
			disp(p2,n);
			sum(p1,p2,n);
			disp(p1,n);
			ret=chksum(p1,n);
			disp(&ret,1);
			if(ret==n-1)
				break;
			del(p,ret,n);
			n--;
		}
		printf("%d\n",p1[ret]);
		free(p);free(p1);free(p2);
	}
	return 0;
}
