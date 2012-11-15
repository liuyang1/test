#include "apue.h"
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>

static void 
err_doit(int,int,const char*,va_list);

void 
err_dump(const char *fmt,...)
{
    printf("func TODO");
}

void 
err_msg(const char *fmt,...)
{
    printf("func TODO");
}

void 
err_quit(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    err_doit(0,0,fmt,ap);
    va_end(ap);
    exit(1);
}

void 
err_exit(int error,const char *fmt,...)
{
    printf("func TODO");
}

void 
err_ret(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    err_doit(1,errno,fmt,ap);
    va_end(ap);
}

void 
err_sys(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    err_doit(1,errno,fmt,ap);
    va_end(ap);
    exit(1);
}


static void 
err_doit(int errnoflag,int error,const char* fmt,va_list ap)
{
    char buf[MAXLINE];
    vsnprintf(buf,MAXLINE,fmt,ap);
    if(errnoflag)
        snprintf(buf+strlen(buf),MAXLINE-strlen(buf),": %s",strerror(error));
    strcat(buf,"\n");
    fflush(stdout);
    fputs(buf,stderr);
    fflush(NULL);// flushes all stdio output streams
}

/*-------------------- log handle func -------------------------*/
void 
log_msg(const char *fmt,...)
{
    printf("func TODO");
}

void 
log_open(const char *ident,int option,int facility)
{
    printf("func TODO");
}

void 
log_quit(const char*fmt,...)
{
    printf("func TODO");
}

void 
log_ret(const char*fmt,...)
{
    printf("func TODO");
}

void 
log_sys(const char*fmt,...)
{
    printf("func TODO");
}

//add
