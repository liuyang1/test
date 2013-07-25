/*
 * ========================================================
 *
 *       Filename:  acc.c
 *
 *    Description:  测试openACC的程序
 *
 *        Version:  1.0
 *        Created:  07/05/2013 10:07:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>
#include <openacc.h>
#define N 100000000

int main(){
    double pi=0.0f;
    long i;
#pragma acc parallel loop reduction (+:pi)
    for(i=0;i<N;i++){
        double t=(double)((i+0.5)/N);
        pi += 4.0/(1+t*t);
    }
    printf("pi=%16.15f\n",pi/N);
    return 0;
}
