/**
 * @file sh.c
 * @brief so simple a shell
 *          which "can't do", is supported by shell.
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.2
 * @date 2012-11-12
 */
#include "apue.h"
#include <sys/wait.h>

//add by 1-8
static void sig_int(int);// system interrupt handle function
// system interrupt
// example: Ctrl+C

    int
main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    //add by 1-8
    if (signal(SIGINT,sig_int) == SIG_ERR)
        err_sys("signal error");

    printf("> ");//promopt
    while (fgets(buf,MAXLINE,stdin) != NULL) {// can't handle backspace and ... key
        if (buf[strlen(buf) -1]=='\n')
            buf[strlen(buf) -1]=0;
        if ((pid=fork()) < 0) {
            err_sys("fork error");
        } else if (pid==0) {// child process
            execlp(buf,buf,(char *)0);// execute cmd
            // can't repalce . .. symbol
            err_ret("couldn't execute: %s",buf);
            exit(127);
        }
        //parent
        //pid!=0
        if ((pid=waitpid(pid,&status,0))<0)
            err_sys("waitpid error");
        printf("> ");
    }
    exit(0);
}

//add by 1-8
    void
sig_int(int signo)
{
    printf("interrupt\n> ");
}
