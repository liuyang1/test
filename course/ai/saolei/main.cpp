#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define M		30		//height
#define N		40		//width width>=height
#define NUM		99		//number of mine
#define MINEFLAG	-1

int				cnter[M][N];	//计数周围的雷的个数,如果本身是雷,则标为-1,否则标记周围雷的个数
double			prob[M][N];		//有雷的估计概率
// 注意有雷的概率,设置一个值-1,表示未被进行估计的所有点的平均概率,这样被点击的时候,减少更新
#define NORMPROB	-1
int				isClk[M][N];	//是否被点击过,0代表未被点击,1代表被点击过

void init(){
	int i,j;
	for(i=0;i!=M;i++){
		for(j=0;j!=N;j++){
			cnter[i][j]=0;
			prob[i][j]=NUM/(M*N+0.0);
		}
	}
	int m,n;
	for(i=0;i!=NUM;i++){
		while(1){
			m=rand()%M;
			n=rand()%N;
			if(cnter[m][n]==0)
				break;
		}
		cnter[m][n]==MINEFLAG;
	}
	int p,q,count=0;
	for(i=0;i!=M;i++){
		for(j=0;j!=N;j++){
			if(cnter[i][j]==MINEFLAG)
				continue;
			count=0;
			for(p=-1;p<=1;p++){
				for(q=-1;q<=1;q++){
					if((p!=0&&q!=0)||(i+p)<0||(i+p)>=M||(j+q)<0||(j+q)>=N)
						count+=cnter[i+p][j+q]==MINEFLAG;
				}
			}
		}
	}

}

//选择点击的点
int LocPoint(int& x,int& y){
}
int updateProb(int i,int j){
	int p,q;
	for(p=-1;p<=1;p++){
		for(q=-1;q<=1;q++){
			if((p!=0&&q!=0)||(i+p)<0||(i+p)>=M||(j+q)<0||(j+q)>=N){
				prob[i+p][j+q]=cnter[i][j]/8.0;
			}
		}
	}
}
//循环点击,
//如果点中雷,则返回0,如果胜利返回1,如果首次点击即为雷,则返回为-1
int loop(){
	//随机点击第一个点
	int p=rand()%M,q=rand()%N;
	//检测是否失败,失败返回-1
	if(cnter[p][q]==MINEFLAG)
		return -1;
	//成功,则处理isClk以及prob
	isClk[p][q]=1;
	updateProb(p,q);
	while(1){
		//如果有概率为0,且未被点击的点,则点之
		for(p=0;p!=M;p++){
			for(q=0;q!=N;q++){
				if(isClk[p][q]==0&&(prob[p][q]<1e-4&&prob[p][q]>1e-4)){
					isClk[p][q]=1;
					updateProb(p,q);
				}
			}
		}
		//选择概率最小的点,点之
		
		//检测是否失败,失败则返回0

		//检测是否成功,成功则返回1
	}
}

int main(){
	srand(time(NULL));
	//printf("%d\n",rand());
	int i;
	int testT=1000;
	int cnt=0,ret;
	for(i=0;i<testT;){
		init();
		ret=loop();
		if(ret==-1)
			continue;
		else{
			i++;
			cnt+=ret;
		}
	}
	printf("ans: %.4f\n",ret/(testT+0.0));
	return 0;
}
