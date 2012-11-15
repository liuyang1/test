/**
 * @file tr.c
 * @brief copy stdin to stdout,aupe 1-2
 *      via read & write (system call)
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-12
 */
#include "apue.h"

// this size is better
// neither so litte that read write too much times
// nor so big that read ahead tech loss efficacy
#define BUFFSIZE    4096

int
main(void)
{
    int n;
    char buf[BUFFSIZE];

    while (( n = read( STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if ( write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    if ( n < 0)
        err_sys("read error");
    exit(0);
}
