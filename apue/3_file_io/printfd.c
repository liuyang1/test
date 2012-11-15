/**
 * @file printfd.c
 * @brief print fd of FILENO 
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-12
 */
#include "apue.h"
#include <fcntl.h>

int main(int argc, char** argv)
{
    int val;

    if (argc!=2)
        err_quit("usage: %s <descriptor#>");

    if ((val=fcntl(atoi(argv[1]),F_GETFL,0)) < 0)
        err_sys("fcntl error for fd %d",atoi(argv[1]));

    switch(val & O_ACCMODE){
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;
        default:
            err_dump("unkown access mode");
    }

    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", noblocking");
#ifdef O_SYNC
    if (val & O_SYNC)
        printf(", synchorous writes");
#endif
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC)
    if (val & O_FSYNC)
        printf(", synchorous writes");
#endif
    putchar('\n');
    return 0;
}
