/**
 * @file tr.c
 * @brief copy stdin to stdout,aupe 1-2
 *      via stdio operator
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-12
 */
#include "apue.h"

int
main(void)
{
    int c;
    while (EOF != (c=getc(stdin)) )
        if (putc(c,stdout) == EOF)
            err_sys("output error");
    if (ferror(stdin))// feeor: return file io error
        err_sys("input error");
    exit(0);
}
