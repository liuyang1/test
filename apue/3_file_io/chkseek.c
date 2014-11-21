/**
 * @file chkseek.c
 * @brief check is CAN seek offset for STDIN
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-12
 */
#include "apue.h"

int main(void)
{
    if (lseek(STDIN_FILENO,0,SEEK_CUR) == -1)
        printf("cannot seek\n");
    else
        printf("seek OK\n");
    // a file or dir CAN seek
    // nothing ,stream and pipe CANNOT seek
    return 0;
}
