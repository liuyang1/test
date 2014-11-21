/*
 * ========================================================
 *
 *       Filename:  1.c
 *
 *    Description:  本文件测试了fd,文件描述符的行为.
 *
 *    int open(char* filename,int flags,mode_t mode);
 *    success: return new fd.
 *    failure: return -1
 *
 *    int close(int fd);
 *    内核释放文件打开时创建的数据结构,并将文件描述符恢复到可用的描述符池中.
 *
 *    这样程序的逻辑,就会返回为3.如果存在baz.txt文件.
 *    如果不存在该文件,则返回为-1.
 *
 *        Version:  1.0
 *        Created:  05/22/2013 05:49:34 PM
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

    fd1=open("foo.txt",O_RDONLY,0);
    printf("fd1=%d\n",fd1);
    close(fd1);
    printf("fd1=%d\n",fd1);
    fd2=open("baz.txt",O_RDONLY,0);
    printf("fd2=%d\n",fd2);
    return 0;
}
