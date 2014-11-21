/**
 * @file pid.c
 * @brief get pid of process
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-12
 */
#include "apue.h"

int
main(void)
{
    printf("process ID %d\n",getpid());
    exit(0);
}
