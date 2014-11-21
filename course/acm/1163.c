#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef double elem_type;

int compare(const void *n1,const void *n2){
	return (*(elem_type*)n1 > *(elem_type*)n2) ?
		1 : -1;
}
elem_type median(elem_type arr[],int n){
	qsort(arr,n,sizeof(elem_type),compare);
	if(n%2==1)
		return arr[n/2];
	else
		return (arr[n/2]+arr[n/2-1])/2.0;
}

int main(){
	int i,N;
	scanf("%d",&N);
	int n,j;
	elem_type *arr,ret;
	for(i=0;i<N;i++){
		scanf("%d",&n);
		arr=(elem_type*)malloc(n*sizeof(elem_type));
		for(j=0;j<n;j++)
			scanf("%lf",arr+j);

		ret=median(arr,n);
		printf("%.3lf\n",ret);

		free(arr);
	}
	return 0;
}
