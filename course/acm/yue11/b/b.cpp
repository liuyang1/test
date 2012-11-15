#include <stdio.h>
#include <map>
using namespace std;
int sum(map<int,int> food,int n1,int n2){
	int ret=0;
	map<int,int>::iterator iter;
	for(iter=food.begin();iter!=food.end();iter++){
		if(iter->first>=n1&&iter->first<=n2)
			ret+=iter->second;
	}
	return ret;
}
int main(){
	char c;
	int k,n;
	map<int,int> food;
	int ret;
	while(EOF!=scanf("%c%d%d",&c,&k,&n)){
		switch(c){
			case 'p':
				food[k]+=n;break;
			case 'c':
				food[k]-=n;break;
			case 'q':
				ret=sum(food,k,n);
				printf("%d\n",ret);
				break;
			default:
				break;
		}
	}
	return 0;
}
