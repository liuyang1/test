/**
 * Based on http://web.eecs.utk.edu/~huangj/cs360/360/notes/Setjmp/sh4.c
 */
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int i, j;
long T0;
jmp_buf Env;

void alarm_handler(int dummy)
{
    long t1;

    t1 = time(0) - T0;
    printf("%ld second%s has passed: j = %d.  i = %d\n", t1,
           (t1 == 1) ? "" : "s", j, i);
    if (t1 >= 8) {
        printf("Giving up\n");
        longjmp(Env, 1);
    }

    alarm(1); // continue to alarm to count time
}

int main() {

    signal(SIGALRM, alarm_handler);
    alarm(1);

    /**
     * 1. first time, return 0, so continue to tic time.
     * 2. When longjmp return back, giveing up.
     */
    if (setjmp(Env) != 0) {
        printf("Gave up:  j = %d, i = %d\n", j, i);
        exit(1);
    }

    T0 = time(0);

    for (j = 0; j < 10000; j++) {
        for (i = 0; i < 1000000; i++) {
            ;
        }
    }
    return 0;
}
