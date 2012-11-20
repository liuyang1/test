/*
 * =============================================================
 *
 *       Filename:  shuffle.c
 *
 *    Description:  洗牌算法
 *
 *        Version:  1.0
 *        Created:  11/20/2012 09:30:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * =============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXLEN	10

long myrand(int x)
{
	return rand()%x;
}
void myShuffle(char* out,int len)
{
	char t;
	size_t rp;
	/*----------------------------------------------------------
	 *  这里是算法的核心.
	 *  http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
	 *
	 *  从0~pos中选择一个数t,然后挑选出来(挑选出来,放到第pos个位置,
	 *  表示已经选过了,之后不会再次选择到..
	 *  而原始在pos位置的数,则和原在t的数互换,供以后选择.
	 *
	 *  这还是一个本地算法inside-out算法.
	 *
	 *  本算法的复杂度为O(n).
	 *
	 *  但是在高阶语言中,
	 *		利用快速的内置排序算法,
	 *		每一个位置设一个随机数,
	 *		然后对该序列进行排序,对应就得到新的洗牌序列
	 *	这种算法的复杂度为O(NlogN),但是实际可能更快.
	 *--------------------------------------------------------*/
	for(size_t pos= MAXLEN-1;pos>0;pos--){
		rp=myrand(pos+1);
		if(rp==pos)
			continue;
		t        = out[rp];
		out[rp]  = out[pos];
		out[pos] = t;
	}
}

void dumpArr(char* p,size_t len)
{
	for(size_t pos=0;pos<len;pos++){
		printf("%c ",p[pos]);
	}
	printf("\n");
}

/**
 * @brief SflStat 统计随机后的结果
 *
 * @param sat	从arr统计结果到sat
 * @param 
 */
void SflStat(int sat[][MAXLEN],char *arr,int len)
{
	char val;
	for(size_t pos=0;pos<len;pos++){
		val=arr[pos];
		sat[pos][val-'0']++;
	}
}

double square(double val)
{
	return val*val;
}
double calcVar(int sat[][MAXLEN],int len){
	int sum=0;
	for(size_t pos=0;pos<len;pos++){
		for(size_t val=0;val<len;val++){
			sum+=sat[pos][val];
		}
	}
	double mean=((double)sum)/len/len;
	double var=0;
	for(size_t pos=0;pos<len;pos++){
		for(size_t val=0;val<len;val++){
			var+=square(sat[pos][val]-mean);
		}
	}
	return var/mean/mean;
}
void dumpStat(int sat[][MAXLEN],int len){
	printf("pos\t");
	for(size_t val=0;val<len;val++){
		printf("%6c ",val+'0');
	}
	printf("\n");
	for(size_t pos=0;pos<len;pos++){
		printf("pos: %d \t",pos);
		for(size_t val=0;val<len;val++){
			printf("%6d ",sat[pos][val]);
		}
		printf("\n");
	}
	printf("\n");
	double var=calcVar(sat,len);
	double sqr=sqrt(var/len/len);
	printf("var: %6f std: %6f %%: %6f\n",var,sqr,0.0);
}
int main()
{
	char TestArr[MAXLEN]={'0','1','2','3','4','5','6','7','8','9'};
	char SflArr[MAXLEN]={0};
	int SflMat[MAXLEN][MAXLEN]={{0}};
	for(int Ncnt=100;Ncnt<1e8;Ncnt*=10){
		for(int cnt=0;cnt<Ncnt;cnt++){
			memcpy(SflArr,TestArr,MAXLEN);
			myShuffle(SflArr,MAXLEN);
			SflStat(SflMat,SflArr,MAXLEN);
		}
		dumpStat(SflMat,MAXLEN);
	}
	return 0;
}
