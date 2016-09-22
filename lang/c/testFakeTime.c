/*
 * This is POC of change time in program, to get fake gettimeofday result.
 * This program need ROOT priveldge, or CAP_SYS_TIME.
 * Make sure not run this program in KEY server.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void longtimefunc()
{
    sleep(3);
}

void moveTime(int sec)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    now.tv_sec += sec;
    int ret = settimeofday(&now, NULL);
    if (ret != 0) {
        printf("settimeofday: ret=%d\n", ret);
        perror("settimeofday:");
    }
}

#define TIC { \
    struct timeval start,stop; \
    gettimeofday(&start,NULL);

#define TOC gettimeofday(&stop,NULL); \
    printf("%lu sec %luusec\n", \
           stop.tv_sec - start.tv_sec, stop.tv_usec -start.tv_usec); \
    }

int main()
{
    // Warning: if settime, must set it back. Else get chaos of time.
    int sec = -1;
    TIC;
    moveTime(sec);
    longtimefunc();
    TOC;

    moveTime(-1 * sec);

    return 0;
}
