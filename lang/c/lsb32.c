/*
 * ========================================================
 *
 *       Filename:  bingcd.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2013 06:53:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>
#include <sys/time.h>

#define TIC {struct timeval start,stop; gettimeofday(&start,NULL);
#define TOC gettimeofday(&stop,NULL);printf("%lu sec %luusec\n",stop.tv_sec - start.tv_sec, stop.tv_usec -start.tv_usec);}

inline int
lsb32_1(unsigned long n)
{
    int ret = 31;
    if (n & 0x0000ffff) {
        ret -=16;
        n &= 0x0000ffff;
    }
    if (n & 0x00ff00ff) {
        ret -= 8;
        n &= 0x00ff00ff;
    }
    if (n & 0x0f0f0f0f) {
        ret -= 4;
        n &= 0x0f0f0f0f;
    }
    if (n & 0x33333333) {
        ret -= 2;
        n &= 0x33333333;
    }
    if (n & 0x55555555) {
        ret -= 1;
    }
    return ret;
}

static unsigned long seq1[5] = { 0x0000ffff, 0x00ff00ff, 0x0f0f0f0f, 0x33333333, 0x55555555};
static unsigned long seq2[5] = { 16, 8, 4, 2, 1};
inline int
lsb32_2(unsigned long n)
{
    int ret = 31;
    int i;
    for(i = 0; i < 5; i++){
        if (n & seq1[i]){
            n &= seq1[i];
            ret -= seq2[i];
        }
    }
    return ret;
}
inline int
lsb32(unsigned long n)
{
    if (n == 0)
        return 32;
    int ret = 0;
    while((n & 0x1) == 0){
        n >>= 1;
        ret++;
    }
    return ret;
}

int testlsb32(){
    unsigned int i = 0;
    printf("0x%08x %d\n", i, lsb32(i));
    for (i = 1; i != 0; i<<=1)
        printf("0x%08x %d\n", i, lsb32(i));
    i = -1;
    printf("0x%08x %d\n", i, lsb32(i));

    return 0;
}

int testPerf(int (*func)(unsigned long)){
    unsigned long i;
    for (i = 0; i < 1000 * 1000 * 1000; i ++)
        func(i);
    return 0;
}
int main(){
    printf("bit version\n");
    TIC;
    testPerf(lsb32);
    TOC;

    printf("while version\n");
    TIC;
    testPerf(lsb32_1);
    TOC;

    printf("loop bit version\n");
    TIC;
    testPerf(lsb32_2);
    TOC;
    return 0;
}
