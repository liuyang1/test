#include "math.h"
#include <stdio.h>
#include <stdlib.h>

#define _DISP
//#define _LABEL
#define EXP 2

struct number{
	int num[4];
	int flag;
};

//double LABEL[13]={360,1440,1260,264,9,480,720,216,8,180,72,6,24};
struct number initarray[5040];

inline void num2p(int num,int *p){
	int i;
	for(i=0;i<4;i++)
		*(p++)=0;
	i=3;
	while(num){
		*(--p)=num%10;
		num=num/10;
	}
}

inline int check1(int * p){
	int i,j;
	for(i=0;i<4;i++){
		for(j=i+1;j<4;j++){
			if(p[i]==p[j])
				return 0;
		}
	}
	return 1;
}

void PreInitArray(){
	int i,j;
	int cnt=0;
	int numt[4];
	//struct number * arrayp=initarray;
	for(i=123;i<=9876;i++){
		num2p(i,numt);
		if(check1(numt)){
			initarray[cnt].flag=1;
			for(j=0;j<4;j++)
			{
				initarray[cnt].num[j]=numt[j];
			}
			cnt++;
		}
	}
#ifdef _LABEL	
/*	for(i=0;i<=4;i++){
		for(j=0;j<=4-i;j++){
			LABEL[i*(11-i)/2+j]= pow((4*(i+j-1.6)*(i+j-1.6)+(i-0.4)*(i-0.4)),-1);
		}
	}
	for(i=0;i<13;i++)
		printf("%9d",i);
	printf("\n");
*/
	for(i=0;i<13;i++)
	{
		LABEL[i]= pow( LABEL[i] ,1.85 ) ;
		printf("%9f",LABEL[i]);
	}
#endif
	printf("\nPre Iint Over!\n");
}

void InitArray(struct number * nump){
	int i,j;
	for(i=0;i<5040;i++){
		for(j=0;j<4;j++)
			nump[i].num[j]=initarray[i].num[j];
		nump[i].flag=1;
	}
}

inline void check2(int * num0,int *numg,int *a,int *b){
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
	(*b)-=(*a);
}

double Division(struct number * array,double cnt,int *nump){
	int hist[15]={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};
	int i;
	//for(i=0;i<15;i++)
	//	hist[i]=0;
	int ta,tb;
	for(i=0;i<5040;i++){
		if(array[i].flag){
			check2(array[i].num,nump,&ta,&tb);
			hist[ta*(11-ta)/2+tb]++;
		}
	}
	double div=0;
	double temp;
	for(i=0;i<13;i++){
		if(hist[i]!=0)
		{
			temp=pow( hist[i], EXP);
#ifdef _LABEL
			temp=LABEL[i]*temp*temp;
#endif
			div+=temp;
		}
	}
	return div;
}

int BestDivision(struct number * array,int count){
	double best=10000*10000+0.0;
	int bestindex=-1;
	double new;
	int i;
	double cnt=0.0;
	// direct
	/*
	for(i=0;i<5040;i++){
		if(array[i].flag!=0)
			return i;
	}*/
	for(i=0;i<5040;i++)
		cnt+=array[i].flag;
//	printf("shengyu cnt:%f\n",cnt);
	if(cnt<1.1){
		for(i=0;i<5040;i++){
			if(array[i].flag)
				return i;
		}
	}
	cnt=cnt/13.0;
	for(i=0;i<5040;i++){
		if( (array[i].flag))
		{
			new=Division(array,cnt,array[i].num);
			if(best>new){
				best=new;
				bestindex=i;
			}
			//if(best==13)
			//	break;
		}
		/*else{
			new2=Division(array,cnt,array[i].num);
			if(best2>new2){
				best2=new;
				bestindex2=i;
			}
		}*/
	}
//	printf("best min:%f\n",best);
	return bestindex;
}

int CCguess(int * num){
	int numg[4];
	int cnt=0;
	int i;
	int a,b,ta,tb;
	int ans;
	struct number array[5040];
	//printf("Begin Init!\n");
	InitArray(array);
	//printf("Init Over!\n");
	for(i=0;i<4;i++)
		numg[i]=i;
	while(1){
		check2(num,numg,&a,&b);
		//printf("a:%d,b:%d\n",a,b);
		cnt++;
		if(a==4&&b==0)
			return cnt;
		if(cnt>9)
			return 0;
		for(i=0;i<5040;i++){
			if(array[i].flag){
				check2(array[i].num,numg,&ta,&tb);
				array[i].flag=(ta==a && tb==b);
			}
		}
//		printf("best Error\n");
		ans=BestDivision(array,cnt);
//		printf("Error: ans:%dcnt:%d\n",ans,cnt);
		for(i=0;i<4;i++)
			numg[i]=array[ans].num[i];
	}
}
int main(){
	PreInitArray();
	int i,j,cnt=0;
	int ans;
	int hist[11];
	for(i=0;i<11;i++)
		hist[i]=0;
#pragma omp parallel for
	for(i=0;i<5040;i++){
		ans=CCguess(initarray[i].num);
		hist[ans]++;
		for(j=0;j<4;j++)
			printf("%d",initarray[i].num[j]);
		printf(",%d ",ans);
		if(ans==0){
			printf("\nError!\n");
			//break;
			exit(1);
		}
		if(i%100==0)
			printf("%5d\n",i);
	}
	printf("time:");
	for(j=1;j<11;j++)
		printf("%5d",j);
	printf("\n     ");
	for(j=1;j<11;j++){
		cnt+=hist[j]*j;
		printf("%5d",hist[j]);
	}
	printf("\naverage cnt:%12f\n",cnt/(5040+0.0));
	return 1;
}
