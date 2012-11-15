#include <stdio.h>
#include <stdlib.h>

int calc1(int*p,int *p1,int n,int m){
	int i,j,k,l;
	for(i=0,j=0;j<m;j++){
		*(p1+i*m+j)=*(p+i*m+j);
		for(k=i+1,l=j+1;k<n&&l<m;k++,l++){
			if(*(p+k*m+l)==0)
				;
			else
				*(p1+k*m+l)=*(p1+(k-1)*m+l-1)+1;
		}
	}
	for(i=1,j=0;i<n;i++){
		*(p1+i*m+j)=*(p+i*m+j);
		for(k=i+1,l=j+1;k<n&&l<m;k++,l++){
			if(*(p+k*m+l)==0)
				;
			else
				*(p1+k*m+l)=*(p1+(k-1)*m+l-1)+1;
		}
	}
	return 0;
}
int calc2(int *p,int *p1,int n,int m){
	int i,j,k,l;
	for(i=0,j=m-1;j>0;j--){
		*(p1+i*m+j)=*(p+i*m+j);
		for(k=i+1,l=j-1;k<n&&l>=0;k++,l--){
			if(*(p+k*m+l)==0)
				;
			else
				*(p1+k*m+l)=*(p1+(k-1)*m+l+1)+1;
		}
	}
	for(i=1,j=m-1;i<n;i++){
		*(p1+i*m+j)=*(p+i*m+j);
		for(k=i+1,l=j-1;k<n&&l>=0;k++,l--){
			if(*(p+k*m+l)==0)
				;
			else
				*(p1+k*m+l)=*(p1+(k-1)*m+l+1)+1;
		}
	}
	return 0;
}
int chk(int *p1,int *p2,int n,int m){
	int i,max=0;
	for(i=0;i<n*m;i++){
		if(*(p1+i)>1&&*(p2+i)>1){
			if(max<*(p1+i)+*(p2+i)){
				max=*(p1+i)+*(p2+i);
			}
		}
	}
	if(max==0)
		return -1;
	else
		return max-1;
}
int disp(int *p,int n,int m){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%d ",*(p+i*m+j));
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}
int main(){
	int N,i,j;
	scanf("%d",&N);
	int n,m,ret;
	for(i=0;i<N;i++){
		scanf("%d%d",&n,&m);
		int *p=(int*)malloc(m*n*sizeof(int));
		int *p1=(int*)malloc(m*n*sizeof(int));
		int *p2=(int*)malloc(m*n*sizeof(int));
		for(j=0;j<n*m;j++){
			scanf("%d",p+j);
		}
		calc1(p,p1,n,m);
		calc2(p,p2,n,m);
//		disp(p,n,m); disp(p1,n,m);disp(p2,n,m);
		ret=chk(p1,p2,n,m);
		free(p);free(p1);free(p2);
		printf("%d\n",ret);
	}
	return 0;
}

