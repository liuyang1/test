/*
 * ========================================================
 *
 *       Filename:  1335.c
 *
 *    Description:  tell me the frequencies!
 *
 *        Version:  1.0
 *        Created:  2012年12月01日 14时57分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>

void stat(char* str)
{
    int ans[128]={0};
    while(*str!=0){
        if(*str=='\n' || *str=='\r')
            goto final;
        int idx=*str;
        if(idx>=128||idx<0)
            goto final;
        ans[idx]++;
final:
        str++;
    }
    int i;
    for(i=0;i<128;i++){
        if(ans[i]!=0)
            printf("%d %d\n",i,ans[i]);
    }
    printf("\n");
}
int main(){
    char str[1002];
    while(EOF!=scanf("%s",str)){
        stat(str); 
    }
    return 0;
}
