#include <stdlib.h>
#include <stdio.h>

void disp(int *a,int N){
	int i;
	for(i=0;i<N;i++)
		printf("%d ",*(a+i));
	printf("\n");
}

int quickSort(int data[],int left,int right,int selectIndex){
	int temp=data[left];
	int ptr=left;
	int i=left+1,j=right;
	if(data[i]<=temp){
		data[ptr]=data[i];
		ptr=i;
	}
	while(i!=j){
		if(data[j]>=temp)
			j--;
		else{
			data[ptr]=data[j];
			ptr=j;
			while(data[i]<=temp&&i!=j)
				i++;
			data[ptr]=data[i];
			ptr=i;
		}
	}
	data[ptr]=temp;
	if(ptr==selectIndex)
		return data[ptr];
	if(left<ptr-1)
		quickSort(data,left,ptr-1,selectIndex);
	if(ptr+1<right)
		quickSort(data,ptr+1,right,selectIndex);
	return 0;
}
int main(){
	int N,i;
	scanf("%d",&N);
	int *a=(int *)malloc(N*sizeof(int));
	for(i=0;i<N;i++){
		scanf("%d",a+i);
	}
	disp(a,N);
	int ret=quickSort(a,0,N-1,5);
	printf("%d\n",ret);
	disp(a,N);
	free(a);
	return 0;
}
