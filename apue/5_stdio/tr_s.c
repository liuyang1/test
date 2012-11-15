/**
 * @file tr_s.c
 * @brief using fgets and fputs to copy stdin to stdout
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-13
 */
/*
 * decrese system call to reduce time using.
 */
#include "apue.h"

int main(void)
{
    char buf[MAXLINE];

    while(fgets(buf,MAXLINE,stdin)!=NULL)
        if(fputs(buf,stdout)==EOF)
            err_sys("output error");
    if(ferror(stdin))
        err_sys("input error");
    return 0;
}
