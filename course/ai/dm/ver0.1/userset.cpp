#include <stdio.h>
#include "myutil.h"
#include "userset.h"
#include "user.h"

UserSet::UserSet(){
}

UserSet::UserSet(char *filename){
	init(filename);
}

UserSet::~UserSet(){
}

int UserSet::init(char *filename){
	FILE* fp=fopen(filename,"r");
	CHK_NULL(fp,"UserSet: could not open file\n");
	
	char buf[BUF_LEN];
	while(!feof(fp)){
		if(false==fgets(buf,BUF_LEN,fp))
			break;
		if(*buf=='0')
			break;
		User tmp(buf);
		if(tmp.chkUser()!=-1)
			m_Data[tmp.getUid()]=tmp;
	}

	fclose(fp);
	return 0;
}

/*************************** stat base code **********************************/
// calc mean of BaseStat bs
// mean(BaseStat)	=	sum(key*value)/sum(value);
double mean(BaseStat bs){
	BaseStat::iterator iter;
	double sum=0,cnt=0;
	for(iter=bs.begin();iter!=bs.end();iter++){
		sum+=iter->first*iter->second;
		cnt+=iter->second;
	}
	return sum/cnt;
}

// var(BaseStat)	=	sum(value*(key-mean(bs))^2)
double variance(BaseStat bs){
	BaseStat::iterator iter;
	double m=mean(bs);
	double sum=0,tmp;
	for(iter=bs.begin();iter!=bs.end();iter++){
		tmp=iter->first-m;
		sum+=iter->second*tmp*tmp;
	}
	return sum;
}

// mode(BastStat)	=	max(iter->value)->key
int mode(BaseStat bs){
	BaseStat::iterator iter=bs.begin();
	int maxvalue=iter->second;
	int maxkey=iter->first;
	for(iter++;iter!=bs.end();iter++){
		if(maxvalue<iter->second){
			maxvalue=iter->second;
			maxkey=iter->first;
		}
	}
	return maxkey;
}

// counter(BaseStat)=	sum(value)
int counter(BaseStat bs){
	BaseStat::iterator iter;
	int sum=0;
	for(iter=bs.begin();iter!=bs.end();iter++){
		sum+=iter->second;
	}
	return sum;
}

// median(BaseStat)	=	cnt/2->key
double median(BaseStat bs){
	BaseStat::iterator iter=bs.begin();
	int sum=counter(bs);
	for(int i=0;i<sum/2-1;i++)
		iter++;
	if(sum%2==0){
		//iter+=sum/2-1;
		return (((iter++)->first)+(iter->first))/2.0;
	}else{
		//iter+=sum/2;
		iter++;
		return iter->first;
	}
}
	
/*************************** stat userset code **********************************/
// 使用函数指针,借用同一个循环过程stat来实现,
// 外部的接口是不同的
BaseStat stat(UserSet *pus,void(*func)(UserSet*,int,BaseStat&)){
	BaseStat ret;
	printf("stat ...\nsize: %d\n",pus->m_Data.size());
	// 这里不能用i,因为这是个映射关系,必须使用iter才可以
	std::map<int,User>::iterator		iter;
	for(iter=pus->m_Data.begin();iter!=pus->m_Data.end();iter++){
		(*func)(pus,iter->first,ret);
	}
	return ret;
}

inline void statAgeIn(UserSet *pus,int i,BaseStat& bs){
	bs[pus->m_Data[i].getAge()]++;
}
BaseStat statAge(UserSet *pus){
	return stat(pus,statAgeIn);
}

inline void statHeightIn(UserSet *pus,int i,BaseStat& bs){
	bs[(pus->m_Data[i]).getHeight()]++;
}
BaseStat statHeight(UserSet *pus){
	return stat(pus,statHeightIn);
}

inline void statMarriageIn(UserSet *pus,int i,BaseStat& bs){
	bs[pus->m_Data[i].getMarriage()]++;
}
BaseStat statMarriage(UserSet *pus){
	return stat(pus,statMarriageIn);
}
void disp(BaseStat *bs){
	BaseStat::iterator iter;
	for(iter=bs->begin();iter!=bs->end();iter++){
		printf("%d : %d\n",iter->first,iter->second);
	}
	printf("disp stat over...\n\n");
}
/*************************** test code **********************************/

int testUserSetSelf(){
	UserSet tmp;
	tmp.init(PROFILE_F_FILE);
	int uid=548392;
	if(tmp.m_Data.count(uid)!=0)
		tmp.m_Data[uid].disp();
	else
		printf("uid isn't existed test\n");
	return 0;
}

int testBaseStat(){
	printf("testBaseStat...\n");
	UserSet tmp;
	tmp.init(PROFILE_M_FILE);
	printf("user set init over...\n");
	BaseStat bs;
	bs=statHeight(&tmp);
	disp(&bs);
	bs.erase(bs.begin(),bs.end());
	
	bs=statAge(&tmp);
	disp(&bs);
	
	bs.erase(bs.begin(),bs.end());
	bs=statMarriage(&tmp);
	disp(&bs);
	return 0;
}
int testUserSet(){
	//testUserSetSelf();
	testBaseStat();
	return 0;
}
