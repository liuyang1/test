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

// lenght of ans 128
void dump(int *ans){
    int seq[128];
    int i,j;
    for(i=0;i<128;i++){
        seq[i]=i;
    }
    for(i=0;i<127;i++){
        for(j=i+1;j<128;j++){
            if(ans[seq[j]]<=ans[seq[i]]){
                int t=seq[i];
                seq[i]=seq[j];
                seq[j]=t;
            }

        }
    }
    for(i=0;i<128;i++){
        if(ans[seq[i]]!=0)
            printf("%d %d\n",seq[i],ans[seq[i]]);
    }
}
void stat(char* str)
{
    int ans[128]={0};
    while(*str!=0){
        int idx=*str;
        if(idx>=128||idx<0||*str=='\n'||*str=='\r')
            goto final;
        ans[idx]++;
final:
        str++;
    }
    dump(ans);
}
int main(){
    char str[1002];
    int isbegin=0;
    while(1){
        if(NULL==fgets(str,1002,stdin)){
            break;
        }
        if(isbegin==1){
            printf("\n");
        }
        stat(str); 
        if(isbegin==0)
            isbegin=1;
    }
    return 0;
}
