/**
 * @file err.c
 * @brief stderr handle,errno setting,and perror stderr output
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-12
 */
#include "apue.h"
#include <errno.h>

int
main(int argc,char **argv)
{
    fprintf(stderr,"EACCES: %s\n",strerror(EACCES));
    errno=EACCES;
    perror("example->");
    errno=ENOENT;// errno glabal var
    perror(argv[0]);
    exit(0);
}

