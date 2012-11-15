#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 2*5+4*1+1+20

int parser(char* str,int* nums,char* ops){
	while(*str!='\0'){
		if(isdigit(*str)){
			*nums=atoi(str);
			str+=*nums>9?2:1;
			nums++;
		}else if(*str=='/' || *str=='*' || *str=='+' || *str=='-'){
			*ops=*str;
			ops++;
			str++;
		}else{
			printf("unknown char\n");
			return -1;
		}
	}
	*ops='\0';
	return 0;
}

int main(){
	char str[MAXLEN];
	int nums[5];
	char ops[5];
	char *p;
	int i,cnt;
	while(EOF!=scanf("%s",str)){
		if(0!=parser(str,nums,ops))
			return -1;
		for(p=ops;*p!='\0';p++){
			if(*p=='*'){
				i=p-ops;
				nums[i+1]=nums[i]*nums[i+1];
				nums[i]=0;
			}
		}
		for(p=ops;*p!='\0';p++){
			if(*p=='/'){
				i=p-ops;
				nums[i+1]=nums[i]/nums[i+1];
				nums[i]=0;
			}
		}
		for(p=ops;*p!='\0';p++){
			if(*p=='-'){
				i=p-ops;
				nums[i+1]=nums[i]-nums[i+1];
				nums[i]=0;
			}
		}
		cnt=0;
		for(i=0;i<(p-ops)+1;i++){
			cnt+=nums[i];
		}
		printf("%d\n",cnt);
	}
	return 0;
}
