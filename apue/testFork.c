/*
 * ========================================================
 *
 *       Filename:  testFork.c
 *
 *    Description:  测试fork程序
 *
 *        Version:  1.0
 *        Created:  05/31/2013 01:24:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include "apue.h"

int main(){
    int i;
    for(i=0;i<2;i++){
        fork();
        //printf("ppid %5d,pid %5d,i %d\n",getppid(),getpid(),i);
        printf("-");
    }
    //wait(NULL);
    //wait(NULL);
    //sleep(100);
    return 0;
}
