/**
 * @file ls.c
 * @brief list all file in dir, aupe 1-1
 * @author liuyang1
 * @version 0.1
 * @date 2012-11-12
 */

#include "apue.h"
#include <dirent.h>

int
main(int argc,char **argv)
{
    DIR        *dp;
    struct dirent *dirp;

    if(argc!=2)
        err_quit("usage: ls dir_name");

    if(NULL==(dp=opendir(argv[1])))
        err_sys("can't open %s",argv[1]);
    while (NULL!=(dirp=readdir(dp)))
        printf("%s\n",dirp->d_name);

    closedir(dp);
    exit(0);
}

