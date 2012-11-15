#include <stdio.h>

int main(){
	char str[10001];
	int k;
	int i;
	int len,headerLen;
	while(1){
		scanf("%d",&k);
		if(k==0)
			return 0;
		scanf("%s",str);
		len=strlen(str);
		for(headerLen=len/k;headerLen>=1;headerLen--){

	return 0;
}
