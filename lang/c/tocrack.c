/*
 * ========================================================
 *
 *       Filename:  tocrack.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年10月16日 11时54分21秒
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
#include <stdlib.h>

char *global = NULL;
void tofool(){
    printf("hack in!\n");
}

void torun(){
    printf("torun\n");
}


int tocrack(void (*p)(void)){
    //printf("origin func:%p\n",p);
    char *s0 = global;
    char str[8];
    
    //printf("s0 %p\n",s0);
    //printf("str%p\n",str);
    //printf("diff %d\n", s0 - str);
    gets(s0);
    //printf("s0:  %s\n",s0);
    strcpy(str,s0);
    
    //printf("s0:  %s\n",s0);
    //printf("str: %s\n",str);

    //printf("hacked func:%p\n",p);
    p();

    // free(s0);
    return 0;
}
int main(){
    global = (char *)malloc(sizeof(char) * 1024);
    tocrack(torun);
    free(global);
    return 0;
}
