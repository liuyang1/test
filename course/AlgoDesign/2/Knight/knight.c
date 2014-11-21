#include"stdio.h"
#define N	36

int top=0;

struct stack
{       
	int x;
	int y;
	int step;
}ma[N*N+1]={{0,0,0}};

void push(int a[][N],int i,int j,int m)
{   
	ma[top].x=i;
	ma[top].y=j;
	ma[top].step=m;
	a[i][j]=++top;
}

int pop(int a[][N])
{       
	int temp;
	top--;
	a[ma[top].x][ma[top].y]=0;
	ma[top].x=0;
	ma[top].y=0;
	temp=ma[top].step+1;
	ma[top].step=0;
	return temp;
}

int judge(int i,int j,int a[][N])              /*判断位置是否可以跳,既要在期盼上,而且还是没有经过过*/
{       
	if(i>=0&&j>=0&&i<N&&j<N&&a[i][j]==0)
		return 1;
	return 0;
}

//返回从(i,j)出发,可以跳得位置个数
int jump(int i,int j,int a[][8])
{       
	int col[]={2,1,-1,-2,-2,-1,1,2};
	int row[]={-1,-2,-2,-1,1,2,2,1};
	int t,ti=i,tj=j,count=0;
	for(t=0;t<8;t++){
		ti+=row[t];tj+=col[t];
		if(judge(ti,tj,a))
			count++;
		ti-=row[t];tj-=col[t];
	}
	return count;
}

//寻找出发的最小路径
//这里的a和b的输入还需要分析下
int sort(int a[8],int b[8])
{       
	int i,min=a[0],t=0;
	for(i=1;i<8;i++){
		if(min>a[i]&&a[i]>-1&&a[i]<8){
			min=a[i];
			t=b[i];
		}
	}
	return t;
}

void disp(int a[][N])
{       
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++)
			printf("%5d",a[i][j]);
		printf("\n");
	}
}

void horse(int x,int y)
{       
	int i=x,j=y,min,ti,tj,t,temp=0,flag=0,temp1=0;
	int count[8],num[8]={0};
	int col[]={2,1,-1,-2,-2,-1,1,2};
	int row[]={-1,-2,-2,-1,1,2,2,1};
	int a[N][N]={{0}};
	for(x=0;x<8;x++)
		count[x]=8;
	push(a,i,j,0);
	while(top<N*N)
	{       
		ti=i;tj=j;temp1=0;flag=0;
		for(x=0;x<8;x++)
			count[x]=8;
		for(t=temp;t<8;t++,temp1++){     /*How many ways for the horse can jump for first time*/
			ti+=row[t];tj+=col[t];
			if(judge(ti,tj,a)){
				count[temp1]=jump(ti,tj,a);
				num[temp1]=t;
				flag=1;
			}
			ti-=row[t];tj-=col[t];
		}
		if(flag){
			min=sort(count,num);
			ti+=row[min];tj+=col[min];
			push(a,ti,tj,min);        /*In the stack*/
			i=ti;j=tj;
			temp=0;
		}
		else{
			temp=pop(a);              /*Return the stack*/
			i=ma[top-1].x;
			j=ma[top-1].y;
		}
	}
	disp(a);
}

int main()
{  
	horse(0,0);
	return 0;
}
