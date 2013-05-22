/*
 * ========================================================
 *
 *       Filename:  2.c
 *
 *    Description:  不同的文件描述符,可以对应到不同的文件表项.
 *    从而各自独立的维护自己的文件偏移位置.
 *    (可以参考3.c)
 *
 *        Version:  1.0
 *        Created:  05/22/2013 06:47:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include "../csapp.h"

int main(){
    int fd1,fd2;
    char c;
    fd1=open("foobar.txt",O_RDONLY,0);
    fd2=open("foobar.txt",O_RDONLY,0);
    read(fd1,&c,1);
    read(fd2,&c,1);
    printf("c= %c\n",c);
    return 0;
}
