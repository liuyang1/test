#include <stdio.h>
#include <stdlib.h>
struct Actor{
	int hp;
	int atk;
	double apn;//atk/n
};

inline int compActor(const void * a,const void * b){
	return -1*((struct Actor*)a)->apn+((struct Actor*)b)->apn;
}

int minDamage(struct Actor v11,struct Actor* monster,int n){
	int i,hp;
	for(i=0;i<n;i++){
		hp=monster[i].hp;
		monster[i].hp = hp/v11.atk;
		if((hp%v11.atk)!=0)
			monster[i].hp++;
		monster[i].apn=((double)monster[i].atk)/monster[i].hp;
	}
	//快速排序
	qsort(monster,n,sizeof(struct Actor),compActor);	
	int cnt=0,hpcnt=0;
	for(i=0;i<n;i++){
		hpcnt+=monster[i].hp;
		cnt+=hpcnt*monster[i].atk;
	}
	return cnt;
}
int main(){
	int i,j;
	int t,n,m;
	scanf("%d",&t);
	for(i=0;i<t;i++){
		scanf("%d%d",&n,&m);
		struct Actor v11={0,m};
		struct Actor* monster=(struct Actor*)malloc(sizeof(struct Actor)*n);
		for(j=0;j<n;j++)
			scanf("%d%d",&monster[j].hp,&monster[j].atk);
		printf("Case #%d: %d\n",i+1,minDamage(v11,monster,n));
		free(monster);
	}
	return 0;
}
