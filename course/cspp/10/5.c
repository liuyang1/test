/*
 * ========================================================
 *
 *       Filename:  5.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/22/2013 07:03:40 PM
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
    read(fd2,&c,1);
    dup2(fd2,fd1);
    read(fd1,&c,1);
    printf("c= %c\n",c);
    return 0;
}
