/**
* @file horse.c
* @brief 跳马问题,也就是求棋盘按跳马规则,寻找哈密顿回路的问题.
		参考了http://blog.csdn.net/eshow/article/details/1780999找到了一个算法复杂度相对较低的思路O(N^2)
		这是可以接受了,就采用这种方法
* @author liuyang1
* @version 
* @date 2012-03-26
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Pos{
	int x;
	int y;
};

#define ORIENN	8
// 从x轴逆时针枚举
const int RuleX[ORIENN]={1,2,-1,-2,-2,-1,1,2};
const int RuleY[ORIENN]={2,1,2,1,-1,-2,-2,-1};

inline int ChkPos(int width,int height,struct Pos pos){
	return pos.x>=0&&pos.y>=0&&pos.x<width&&pos.y<height;
}
inline int ChkPassed(int width,int height,struct Pos pos,int *PassedFlag){
	return *(PassedFlag+width*pos.x+pos.y);
}
inline void SetPassedFlag(int width,int height,struct Pos pos,int *PassedFlag){
	*(PassedFlag+width*pos.x+pos.y)=1;
}
inline void UnsetPassedFlag(int width,int height,struct Pos pos,int *PassedFlag){
	*(PassedFlag+width*pos.x+pos.y)=0;
}
int DepSearchHalmitonHorse(int width,int height,struct Pos *seq,int count,int *PassedFlag);

int AddPos(int width,int height,struct Pos *seq,int count,int *PassedFlag,struct Pos PosNew){
	if(ChkPos(width,height,PosNew)&&(0==ChkPassed(width,height,PosNew,PassedFlag))){
		//printf("search from %d %d to %d %d\n",PosNow.x,PosNow.y,PosNew.x,PosNew.y);
		SetPassedFlag(width,height,PosNew,PassedFlag);
		*(seq+count)=PosNew;
		int ret=DepSearchHalmitonHorse(width,height,seq,count+1,PassedFlag);
		if(ret==1)
			return 1;
		UnsetPassedFlag(width,height,PosNew,PassedFlag);
	}
	return 0;
}
inline void swap(int *a,int *b){ int t=*a;		*a=*b;		*b=t; }
inline int min(int a,int b){ return a<b?a:b; }
inline int max(int a,int b){ return a>b?a:b; }
// 深度优先搜索计算一个结果出来
// 输入,高和宽,以及点序列的指针,这里指针需要预先申请足够的空间
// 返回,成功与否
// 点序列的内容到点序列的指针
int DepSearchHalmitonHorse(int width,int height,struct Pos *seq,int count,int *PassedFlag)
{
	if(count==width*height)
		return 1;//找到了
	struct Pos PosNow=*(seq+count-1);
	struct Pos PosNew;
	
	int i,j;
	int cnt=0;
	int testseq[8];
	int val[8];
	for(i=0;i<8;i++)
		testseq[i]=i;
	for(i=0;i<8;i++){
		PosNew.x=PosNow.x+RuleX[i];
		PosNew.y=PosNow.y+RuleY[i];
		if(ChkPos(width,height,PosNew)&&!ChkPassed(width,height,PosNew,PassedFlag)){
			cnt=0;
			for(j=0;j<8;j++){
				struct Pos test;
				test.x=PosNew.x+RuleX[i];
				test.y=PosNew.y+RuleY[i];
				//SetPassedFlag(width,height,test,PassedFlag);
				if(ChkPos(width,height,test)&&!ChkPassed(width,height,test,PassedFlag)){
					cnt++;
				}
				//UnsetPassedFlag(width,height,test,PassedFlag);
			}
			val[i]=cnt;
		}else{
			val[i]=9;
		}
	}
	for(i=0;i<8;i++){
		for(j=i+1;j<8;j++){
			if(val[i]>val[j]){
				swap(val+i,val+j);
				swap(testseq+i,testseq+j);
			}
		}
	}

	for(i=0;i<ORIENN;i++){
		PosNew.x=PosNow.x+RuleX[testseq[i]];
		PosNew.y=PosNow.y+RuleY[testseq[i]];
		int ret=AddPos(width,height,seq,count,PassedFlag,PosNew);
		if(ret==1)
			return 1;
	}
	return 0;//找不到
}

inline int ChkSize(int width,int height){ return width%2==0&&height%2==0&&(width==height||abs(width-height)==2); }
void dispSeq(int width,int height,struct Pos *seq){
	int i;
	for(i=0;i<width*height;i++){
		printf("(%2d,%2d) ",seq[i].x,seq[i].y);
		if(i%width==width-1)
			printf("\n");
	}
}
void dispOrder(int width,int height,struct Pos *seq,int *PassedFlag){
	int i,j;
	for(i=0;i<width*height;i++){
		PassedFlag[seq[i].x*(width)+seq[i].y]=i+1;
	}
	for(i=0;i<width;i++){
		for(j=0;j<height;j++){
			printf("%2d ",PassedFlag[i*width+j]);
		}
		printf("\n");
	}
}
int HalmitionHorse(int width,int height){
	if(!ChkSize(width,height)){
		printf("could not find!\n");
		return 0;
	}
	struct Pos *seq=(struct Pos*)malloc(sizeof(struct Pos)*width*height);
	int *PassedFlag=(int *)malloc(sizeof(int)*width*height);
	int i;
	for(i=0;i<width*height;i++)
		*(PassedFlag+i)=0;

	struct Pos origin;
	origin.x=0;		origin.y=0;
	SetPassedFlag(width,height,origin,PassedFlag);
	*(seq+0)=origin;
	int ret=DepSearchHalmitonHorse(width,height,seq,1,PassedFlag);
	if(ret!=1){
		printf("could not find!\n");
	}
	dispSeq(width,height,seq);
	dispOrder(width,height,seq,PassedFlag);
	//free(PassedFlag);//this is a ERROR.
	//free(seq);
	return 0;
}
// 计算特定大小的接口函数
// 首次调用,进行计算,之后将结果保存在内部,直接返回

// 对特别大的情况,进行划分

// 对已经进行的划分,进行拼接

int main(){
	int m,n;
	scanf("%d%d",&m,&n);
	HalmitionHorse(m,n);
	return 0;
}
