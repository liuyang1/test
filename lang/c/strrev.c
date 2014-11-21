/*
 * ========================================================
 *
 *       Filename:  strrev.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年09月08日 20时38分09秒
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

void strrev(char* p){
    int n = strlen(p);
    int i;
    char t;
    for(i=0;i<n/2;i++){
        t = p[i];
        p[i] = p[n-i-1];
        p[n-i-1] = t;
    }
    return;
}

int main(){
    char s[] ="abcdefg";
    char s2[] = "abcdef";
    strrev(s);
    strrev(s2);
    printf("%s\n",s);
    printf("%s\n",s2);
    return 0;
}
