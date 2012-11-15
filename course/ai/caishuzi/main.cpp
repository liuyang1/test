#include <stdio.h>
#include <omp.h>

#define N		5040
#define VIC		44
#define POSS	1
#define UNPOSS	0

int num[N];
int flag[N];
int branch[VIC+1];

int isNum(int n){
	int a[4],i,j;
	for(i=0;i!=4;i++){
		a[i]=n%10;
		n=n/10;
	}
	for(i=0;i!=3;i++){
		for(j=i+1;j!=4;j++){
			if(a[i]==a[j])
				return 0;
		}
	}
	return 1;
}

void init(){
	int i,cnt=0;
	for(i=123;i<=9876;i++){
		if(isNum(i)){
			num[cnt]=i;
			cnt++;
		}
	}
}

int cai(int n1,int n2){
	int a=0,b=0;
	int nn1[4],nn2[4];
	int i,j;
	for(i=0;i!=4;i++){
		nn1[i]=n1%10;
		nn2[i]=n2%10;
		n1=n1/10;
		n2=n2/10;
	}
	for(i=0;i!=4;i++){
		if(nn1[i]==nn2[i])
			a++;
	}
	for(i=0;i!=4;i++){
		for(j=0;j!=4;j++){
			if(nn1[i]==nn2[j])
				b++;
		}
	}
	return a*10+b;
}

// 选择猜测的数字
int selectByMinMax(){
	int i,j,k,max,min,yi=123;
	min=N;
	int sum=0,index=0;
	for(i=0;i!=N;i++){
		if(flag[i]==POSS){
			sum++;
			index=i;
		}
	}
	if(sum==1)
		return num[index];
	//printf("\tposs cnt: %d\n",sum);

	for(i=0;i!=N;i++){
		if(flag[i]==UNPOSS)
			continue;
		for(k=0;k<=VIC+1;k++)
			branch[k]=0;
		for(j=0;j!=N;j++){
			if(flag[j]==POSS)
				branch[cai(num[j],num[i])]++;
		}
		max=0;
		for(k=0;k<=VIC+1;k++){
			if(max<branch[k]){
				max=branch[k];
			}
		}
		if(min>max){
			min=max;
			yi=num[i];
		}
	}
	for(i=0;i!=N;i++){
		if(flag[i]==UNPOSS){
			for(k=0;k<=VIC+1;k++)
				branch[k]=0;
			for(j=0;j!=N;j++){
				if(flag[j]==POSS)
					branch[cai(num[j],num[i])]++;
			}
			max=0;
			for(k=0;k<=VIC+1;k++){
				if(max<branch[k]){
					max=branch[k];
				}
			}
			if(min>max){
				min=max;
				yi=num[i];
			}
		}
	}
	//printf("minmax: %d\n",min);
	return yi;
}

int select(){
	return selectByMinMax();
}

int loop(int jia){
	int ret=1,ans;
	int i;
	for(i=0;i!=N;i++){
		flag[i]=POSS;
	}
	ans=cai(jia,123);
	if(ans==VIC)
		return ret;
	for(i=0;i!=N;i++){
		if(flag[i]==POSS&&cai(num[i],123)!=ans){
			flag[i]=UNPOSS;
		}
	}
	while(1){
		int yi=select();
		ans=cai(jia,yi);
		//printf("yi: %d ans: %d\n",yi,ans);
		ret++;
		if(ans==VIC)
			return ret;
		for(i=0;i!=N;i++){
			if(flag[i]==POSS&&cai(num[i],yi)!=ans){
				flag[i]=UNPOSS;
			}
		}
		if(ret>=15)
			return -1;
	}
}

int run(){
	init();
	int cnt=0,ret;
#pragma omp parallel for
	for(int i=0;i!=N;i++){
		printf("%d\n",num[i]);
		//cnt+=loop(num[i]);
		ret=loop(num[i]);
		if(ret==-1){
			printf("could guess number %d\n",num[i]);
			return 0;
		}
		cnt+=ret;
	}
	printf("%d %.4f\n",cnt,cnt/5040.0);
	return 0;
}

//测试,猜测一个数字
int testCaiOne(){
	init();
	printf("%d\n",loop(2345));
	return 0;
}
int testGenNum(){
	init();
	int i;
	for(i=0;i!=N;i++){
		printf("%d\n",num[i]);
	}
	return 0;
}
int testCai(){
	int ret;
	ret=cai(123,123);printf("%d\n",ret);
	ret=cai(2345,5687);printf("%d\n",ret);
	ret=cai(1345,7982);printf("%d\n",ret);
	ret=cai(8012,1246);printf("%d\n",ret);
	ret=cai(8012,812);printf("%d\n",ret);
	return 0;
}
int test(){
	testCaiOne();
	return 0;
}
int main(){
	//test();
	run();
	return 0;
}
