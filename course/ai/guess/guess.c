#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define _DISP	0 
void disp(int * num1){
#ifndef _DISP
	printf( "num: %d %d %d %d\n",num1[0],num1[1],num1[2],num1[3]);
#endif
}

inline void num2p(int seqn, int * nump){
//数字转为数组,并用指针传出
	int i;
	for(i=0; i<4; i++)
		nump[i]=0;
	i=3;
	while(seqn){
		nump[i--]=seqn%10;
		seqn=seqn/10;
	}
}

int p2num(int * nump){
//数组转化为数字
	int i,num=0;
	for(i=0;i<4;i++)
		num=10*num+nump[i];
	return num;
}

inline int check1( int * nump){
//检查数组生成是否符合规则
//1:	符合规则
//0:	不符合规则
	int i,j;
	for(i=0;i<4;i++){
		for(j=i+1;j<4;j++){
			if(nump[i]==nump[j])
				return 0;
		}
	}
	return 1;
}

void randnum(int * nump){
//随机产生数字
	int array[10]={0,1,2,3,4,5,6,7,8,9};
	int i;
	int tmp,rd;
	for(i=0; i<4; i++){
		rd=rand()%10;
		tmp = array[rd];
		array[rd] = array[9-i];
		array[9-i] = tmp;
	}
	for(i=0; i<4; i++){
		nump[i]=array[6+i];
	}
}

inline void seqnum(int * nump){
//顺序产生数字
	static int cnt=0;
	cnt++;
	static int seqn=0;
	while(1){
		seqn++;
		num2p(seqn,nump);
		if(check1(nump)==1)
			break;
	}
}

inline void check2(int * num0,int * numg,int * a, int *b){
//检查数组于猜测数组吻合程度
//a,数字相同,位置相同
//b,数字相同,位置不同
	int i,j;
	*a=0;
	*b=0;
	for(i=0;i<4;i++){
		if(num0[i]==numg[i])
			(*a)++;
		for(j=0;j<4;j++){
			if(num0[i]==numg[j])
				(*b)++;
		}
	}
	(*b)=(*b)-(*a);
}
/*
void getinput(int * nump){
	static int cnt=0;
	while(1){
		printf( "Please Input Your Number!\n");
		int num;
		scanf("%d",&num);
		if(num<123 || num >9876){
			printf( "Wrong Format Number!\n");
			cnt++;
			if( cnt==3){
				printf( "Wrong Formant Too Many!\nExit!\n");
				exit(0);
			}
			continue;
		}
		num2p(num,nump);
		if(check1(nump))
			return;
		cnt++;
		printf( "Wrong Format Number!\n");
		if(cnt>=3){
			printf( "Wrong Format Too Many!\nExit!\n");
			exit(0);
		}
	}
}

void CPguess(){
//电脑随机数字,玩家进行游戏
	int num[4];
	srand(time(NULL));
	randnum( num);
	disp(num);
	int numg[4];
	int cnt=0;
	int a,b;
	while(1){
		cnt++;
		getinput(numg);
		check2(num,numg,&a,&b);
		printf( "a:%d	b:%d\n",a,b);
		if(a==4){
			printf( "You Got it!\n%d times!\n",cnt);
			return;
		}
		if(cnt>10){
			printf( "Too Many Times Guess!\n");
			return;
		}
	}
}
*/
int Division(int * array,int * nump){
//nump数组将array数组分类,返回分类的区分度
	int hist[15];
	int i;
	for(i=0;i<15;i++)
		hist[i]=0;
	int numt[4];
	int ta,tb;
	int cnt=0;
	for(i=0;i<9999;i++){
		if(array[i]==1){
			num2p(i,numt);
			check2(numt,nump,&ta,&tb);
			hist[ta*(11-ta)/2+tb]++;
			cnt++;
		}
	}
	int div=0;
	cnt=cnt/14;
	for(i=0;i<13;i++)
		div+=(hist[i]-cnt)*(hist[i]-cnt);
	div+=(hist[14]-cnt)*(hist[14]-cnt);
	return div;
}

int BestDivision(int *array){
	int best=100000000;
	int bestindex=-1;
	int new;
	int i;
	int numt[4];
	int cnt=0;
	for(i=0;i<9999;i++){
		if(array[i]==1)
		{
			num2p(i,numt);
			if(check1(numt)==1){
				new=Division(array,numt);
				if(best>new){
					best=new;
					bestindex=i;
				}
			}
			cnt++;
		}
	}
#ifndef _DISP
	printf( "array cnt:%d\n",cnt);
#endif
	return bestindex;
}

int CCguess(){
	int num[4];
	//srand(time(NULL));randnum(num);
	seqnum(num);
	disp(num);
	int numg[4];
	int cnt=0,i;
	//int j,k;
	int a,b,ta,tb;

	int numt[4];
	int array[9999];
	for(i=0;i<9999;i++){
		num2p(i,numt);
		array[i]=check1(numt);
	}

	for(i=0;i<4;i++)
		numg[i]=i;
	while(1){
		disp(numg);
		check2(num,numg,&a,&b);
		cnt++;
#ifndef	_DISP
		printf( "cnt:%d	a:%d	b:%d\n",cnt,a,b);
#endif
		if(a==4&&b==0){
#ifndef _DISP
			printf( "You Got it!\nFinal CNT: %d\n",cnt);
#endif
			return cnt;
		}
		if( cnt>10){
#ifndef _DISP
			printf( "Too Many Times!\n");
#endif
			return 0;
		}
		//从array剔除不可能为真的数字
		for(i=0;i<9999;i++){
			if(array[i]){
				num2p(i,numt);
				check2(numt,numg,&ta,&tb);
				//if(cnt==2)printf( "i: %d,ta: %d,tb: %d|",i,ta,tb);
				//if(i%6==5)printf( "\n");
				if(ta!=a || tb!=b)
					array[i]=0;
			}
		}
		/*
		int tmp=0;
		for(i=0;i<9999;i++){
			if(array[i]==1){
				printf(" 1");
				tmp++;
			}
			else
				printf("  ");
			if(i%70==69)
				printf("$\n");
		}
		printf( "\ncnt 1: %d\n",tmp);
		*/
		//根据剩余的可能值,推测下一个numg
		int ans=BestDivision(array);
		num2p(ans,numg);
	}
}

int main(){
	int i,cnt=0;
	int ans;
	int hist[11];
	for(i=0;i<11;i++)
		hist[i]=0;
#pragma omp parallel for
	for(i=0;i<5040;i++){
		ans=CCguess();
		if(ans==0){
			printf("\nerror,%d,%d",i,ans);
			exit(1);
		}
		//printf( "%5d,ans:%d\n",i,ans);
		printf("%5d,%d",i,ans);
		if(i%10==9)
			printf("\n");
		cnt+=ans;
		hist[ans]++;
	}
	printf( "average cnt:%f,cnt:%d,i:%d\n",cnt/(i+0.0),cnt,i);
	for(i=0;i<11;i++)
		printf("time:%3d:%4d\n",i,hist[i]);
	return 1;
}
