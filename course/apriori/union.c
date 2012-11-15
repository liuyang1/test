#include "stdio.h"

int main(){
	int seq1[]={1,2,3,4,6,7,10,14};
	int seq2[]={2,4,6,10,15};
	int len1=6;
	int len2=5;
	int i=0,j=0;
	while(1){
		if(seq1[i]<seq2[j]){
			i++;
		}else if(seq1[i]==seq2[j]){
			printf("%d\t",seq1[i]);
			i++;j++;
		}else{
			j++;
		}
		if(i==len1||j==len2){
			break;
		}
	}
	putchar('\n');
	return 1;
}
