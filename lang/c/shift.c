/*
 * ========================================================
 *
 *       Filename:  shift.c
 *
 *    Description:  字符串偏移程序,时间复杂度O(n),空间复杂度O(1)
 *
 *        Version:  1.0
 *        Created:  01/11/2013 10:24:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>
#include <string.h>

inline void swap(char* a,char* b){
	char c;
	c=*a;
	*a=*b;
	*b=c;
}
inline void reverse(char* str,int n){
	int i;
	for(i=0;i<n/2;i++){
		swap(str+i,str+n-1-i);
	}
}
void shift(char* str,int n){
	if(n==0)
		return;
	int len=strlen(str);
	n=n%len;
	reverse(str,n);
	printf("%s\n",str);
	reverse(str+n,len-n);
	printf("%s\n",str);
	reverse(str,len);
	printf("%s\n",str);
	return;
}

int main(){
	char str[]="0123456789";
	int n=4;
	printf("origin:     %s\n",str);
	shift(str,n);
	printf("shift %d:    %s\n",n,str);
	return 0;
}
