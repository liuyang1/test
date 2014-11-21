/*
 * ========================================================
 *
 *       Filename:  3.c
 *
 *    Description:  测试了父子进程,对同一个文件的描述符操作的情况
 *
 *    文件描述符(进程独享)  <->   文件表(所有进程共享) <-> vnode表
 *
 *    文件的偏移位置信息存储在文件表中.
 *
 *    父子进程会共享文件表.因此如果在第一个进程进行了读写操作,那么影响到文件的偏移位置.
 *
 *        Version:  1.0
 *        Created:  05/22/2013 06:49:34 PM
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
    int fd;
    char c;
    fd=open("foobar.txt",O_RDONLY,0);
    pid_t p;
    if((p=fork())==0){
        // child process
        printf("pid: %d\n",p);
        read(fd,&c,1);
        return 0;
    }
    wait(NULL);
    // int wait(int* state);
    // if child process change state,this func return immediatly
    // so this func will wait until child process return
    printf("child pid: %d\n",p);
    read(fd,&c,1);
    printf("c=%c\n",c);
    return 0;
}
