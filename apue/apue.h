#ifndef _AUPE_H_
#define _AUPE_H_

#define _XOPEN_SOURCE	600

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>

#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define MAXLINE     4096
void err_dump(const char *,...);
void err_msg(const char *,...);
void err_quit(const char *,...);
void err_exit(int,const char *,...);
void err_ret(const char *,...);
void err_sys(const char *,...);

void log_msg(const char *,...);
void log_open(const char *,int,int);
void log_quit(const char*,...);
void log_ret(const char*,...);
void log_sys(const char*,...);

#endif/*END HEADER*/
