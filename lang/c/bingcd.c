/*
 * ========================================================
 *
 *       Filename:  bingcd.c
 *
 *    Description:  GCD算法,求最大公约数
 *                  二进制算法,并没有从根本上提升其复杂度.
 *                  只是利用二进制运算的特性,使得速率有常数倍的提升.
 *                  GCD算法的复杂度一般为log(min(a,b)**2.
 *                  注意,这里的平方,是考虑到求余运算所需要的时间是log(min(a,b))的.
 *                  如果认为求余是常数时间,复杂度则为log(min(a,b))即可.
 *
 *        Version:  1.0
 *        Created:  10/25/2013 07:34:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define TIC {struct timeval start,stop; gettimeofday(&start,NULL);
#define TOC gettimeofday(&stop,NULL);printf("%lu sec %luusec\n",stop.tv_sec - start.tv_sec, stop.tv_usec -start.tv_usec);}

inline int
lsb32(unsigned long n)
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

int
bingcd(int a, int b)
{
    if (a == 0 && b == 0) {
        return 0;
    }
    int t;
    int alsb, blsb, minlsb;
    alsb = lsb32(a);
    blsb = lsb32(b);
    minlsb = alsb < blsb ? alsb : blsb;
    a >>= minlsb;
    b >>= minlsb;
    while(1) {
        if (b == 0)
            return a << minlsb;
        a >>= lsb32(a);
        b >>= lsb32(b);
        t = a;
        a = b;
        b = t % b;
    }
}

int
gcd(int a, int b)
{
    if (a == 0 && b == 0) {
        return 0;
    }
    int t;
    while(1) {
        if (b == 0)
            return a;
        t = a;
        a = b;
        b = t % a;
    }
}

int
testLoop(int (*f)(int, int)){
    int i;
    for (i = 0; i < 1000 * 1000 * 10; i++)
        f(random(), random());
    return 0;
}


int testRight(){
    printf("%d\n",gcd(12, 18));
    printf("%d\n",gcd(18, 12));
    return 0;
}

int testPerf(){
    TIC;
    testLoop(gcd);
    TOC;

    TIC;
    testLoop(bingcd);
    TOC;
    return 0;
}
    
int main(){
    testPerf();
    return 0;
}
