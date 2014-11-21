/*
 * =================================================================
 *
 *       Filename:  testflow.cpp
 *
 *    Description:  just want to test cflow
 *
 *        Version:  1.0
 *        Created:  2012.07.14 17:36:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *        Company:  auto@ustc
 *
 * =================================================================
 */
#include <stdio.h>
int fun1(int n){
	if(n<0)
		return 0;
	if(n==0 || n==1)
		return 1;
	return fun1(n-1)+fun1(n-2);
}
int main(){
	int n;
	while(1){
		scanf("%d",&n);
		printf("ret: %d\n",fun1(n));
	}
	return 0;
}
