#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "myutil.h"
#include "train.h"
#include "user.h"
#include "userset.h"
#include "action.h"

using namespace std;

#define ELO_K	40			// 校正elo值的比例因子
#define ELO_DIV	400			// logistic 概率分布的因子
/******************************** ndcg 基础代码 *********************/
#define RANK_N	10
int ACTION2SCORE[]={0,1,3};	// (2^i)-1	行为到得分
double RANK2WEIGHT[RANK_N];		// 得分的位置权值
double RANK2ACCUMU[RANK_N];		// 得分的位置权值累加
// 初始化权值和权值累加
inline void initWeight(){
	double acc=0;
	for(int i=0;i<RANK_N;i++){
		RANK2WEIGHT[i]=log2(1+1+i);
		acc+=RANK2WEIGHT[i];
		RANK2ACCUMU[i]=acc;
	}
}
inline int Action2Score(int action){
	return ACTION2SCORE[action];
}
inline double Rank2Weight(int rank){
	return rank>RANK_N ? 0 : RANK2WEIGHT[rank];
}
inline double Rank2Accumu(int rank){
	return rank>RANK_N ? RANK2ACCUMU[RANK_N-1] : RANK2ACCUMU[rank];
}

/****************************** train的基本代码 ********************/
Train::Train()
{
	//initWeight();
}

Train::~Train(){
}

int Train::init(char *profile_f,char *profile_m,char *train,char *test){
	initWeight();
	printf("init profile female file...\n");
	m_Users_f.init(profile_f);
	printf("init profile male file...\n");
	m_Users_m.init(profile_m);
	printf("init train file...\n");
	m_Actions.init(train);
	printf("init test file...\n");
	m_Test.init(test);
	printf("train init over\n\n");
	disp();
	return 0;
}

/***************************** ndcg calc ****************************/
// NGCG 计算
double ndcg10(int *actionSeq,int n){
	int i;
	n=n>10?10:n;
	double sum=0,sum0=0;
	// dcg 10
	for(i=0;i<n;i++){
		sum+=Action2Score(actionSeq[i])*Rank2Weight(i+1);
	}
	// opt dcg10
	qsort(actionSeq,n,sizeof(int),int_bigger);	
	for(i=0;i<n;i++){
		sum0+=Action2Score(actionSeq[i])*Rank2Weight(i+1);
	}
	return sum/sum0;
}
/*============================ 计算Elo 排名算法 ======================*/
// elo值到排序
// 返回排名从0~n-1,存储在pSeq内
void EloValue2seq(double *pElo,int  *pSeq,int n){
	int i,j,index=-1;
	double max=0;
	for(i=0;i<n;i++)
		pSeq[i]=-1;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(pSeq[j]!=-1)
				continue;
			if(max<pElo[j]){
				max=pElo[j];
				index=j;
			}
		}
		pSeq[index]=i;
	}			
}
void Seq2score(int *pSeq,double *pSco,int n){
	int i;
	double accum=Rank2Accumu(n);
	for(i=0;i<n;i++)
		pSco[i]=Rank2Weight(i)/accum;
}
// elo value to score
void EloValue2score(double *pElo,double *pSco,int n){
	int pSeq[50];
	//int *pSeq=new int[n];
	//int *pSeq=(int *)malloc(sizeof(int)*n);
	EloValue2seq(pElo,pSeq,n);
	Seq2score(pSeq,pSco,n);
	//free(pSeq);
	//delete []pSeq;
}
// 行为序列到排序
void action2seq(int *pAction,int  *pSeq,int n){
	int i=0,j=0;
	int *pA,*pS;
	for(int action=2;action>0;action--){	
		for(j=0,pA=pAction,pS=pSeq;j<n;j++,pA++,pS++){
			if(*pA==action){
				*pS=i;
				i++;
			}
		}
	}
	if(i!=n){
		printf("error action seq\n");
		exit(-2);
	}
}
// 行为到得分
void action2score(int *pAction,double *pSoc,int n){
	int i,sum=0;
	for(i=0;i<n;i++)
		sum+=(pAction[i]!=0);
	double soc=1/sum;
	// 胜者得分平分1
	for(i=0;i<n;i++)
		pSoc[i]=(pAction[i]!=0?soc:0);
}
// 排名顺序与胜率的映射关系
void seq2rating(double *pSeq,double *pRating,int n){
}
// 多人Elo算法
void MultiElo(UserSet *pus,ActionSet *pas,int fromId,int round)
try
{
	//double fromIdElo=us.getEloValue(fromId);
	unsigned int n=pas->m_Data.size();
	double ptoIdElo[50];
	//double *ptoIdElo=new double[n];
	//double *ptoIdElo=(double *)malloc(sizeof(n));
//	double meanIdElo=0;
	for(unsigned int i=0;i<n;i++){
		ptoIdElo[i]=pus->getEloValue(pas->m_Data[i].m_toId);
//		meanIdElo+=ptoIdElo[i];
	}
//	meanIdElo/=as.m_Data.size();
	
	int pActions[50];
	//int *pActions=new int[n];
	//int *pActions=(int *)malloc(n*sizeof(n));
	for(unsigned int i=0;i<n;i++){
		pActions[i]=pas->m_Data[i].m_action;
	}

	double pERating[50];
	double pRRating[50];
	//double *pERating=new double[n];
	//double *pRRating=new double[n];
	//double *pERating=(double *)malloc(sizeof(n));		//	期望胜率
	//double *pRRating=(double *)malloc(sizeof(n));		//	实际胜率
	
	EloValue2score(ptoIdElo,pERating,n);
	action2score(pActions,pRRating,n);

	// 校正概率
	// R'=R+K(PR-PE)
	double newElo;
	for(unsigned int i=0;i<n;i++){
		newElo=ptoIdElo[i]+ELO_K*(pRRating-pERating);
		pus->setEloValue(pas->m_Data[i].m_toId,newElo);
	}

	/*
	free(pActions);
	free(ptoIdElo);
	free(pERating);
	free(pRRating);
	*/
	/*
	delete []pActions;
	delete []ptoIdElo;
	delete []pERating;
	delete []pRRating;
	*/
}
catch(exception &e){
	cerr<<e.what()<<endl;
	//return -1;
}
// 根据ActionSet对UserSet进行Elo排名计算
// 这里主要是根据formid和round进行了筛选
// 算法的真正实现在MultiElo内部
/*
void EloRating(UserSet &us,ActionSet as){
	ActionSet as0,as1;
	int id;
	std::map<int,User>::iterator iter;
	for(iter=us.m_Data.begin();iter!=us.m_Data.end();iter++){
		id=iter->first;
		as0=as.selectByfromId(id);
		for(int i=0;i<5;i++){
			as1=as.selectByround(i);
			if(as1.size()==0)
				continue;
			MultiElo(us,as,id,i);
		}
	}
}
*/
void Train::EloRating(){
	ActionSet as0,as1;
	int id;
	std::map<int,User>::iterator iter;
	int i;
	int cnt=0;
	//unsigned int maxg=0;
	for(iter=m_Users_f.m_Data.begin();iter!=m_Users_f.m_Data.end();iter++){
		cnt++;
		id=iter->first;
		as0=m_Actions.selectByfromId(id);
		if(as0.size()==0)
			continue;
		printf("%d : %d %d\n",cnt,id,as0.size());
		i=1;
		while(1){
			as1=as0.selectByround(i);
			if(as1.size()==0)
				break;
			//printf("\t%d : %d\n",i,as1.size());
			//if(as1.size()>maxg)
			//	maxg=as1.size();
			MultiElo(&m_Users_m,&as1,id,i);
			i++;
		}
	}
	for(iter=m_Users_m.m_Data.begin();iter!=m_Users_m.m_Data.end();iter++){
		cnt++;
		id=iter->first;
		as0=m_Actions.selectByfromId(id);
		if(as0.size()==0)
			continue;
		printf("%d : %d %d\n",cnt,id,as0.size());
		i=1;
		while(1){
			as1=as0.selectByround(i);
			if(as1.size()==0)
				break;
			//if(as1.size()>maxg)
			//	maxg=as1.size();
			MultiElo(&m_Users_f,&as1,id,i);
			i++;
		}
	}
	//printf("maxg: %d\n",maxg);
	fflush(stdout);
}
/**************************** test code ********************************/
void testTrainBase(){
	Train trn;
	trn.init(PROFILE_F_FILE,PROFILE_M_FILE,TRAIN_FILE,TEST_FILE);
	trn.EloRating();
	getchar();
}
void testTrain()
{
	testTrainBase();
}
