#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    printf("mono\t%ld.%03lus\n",
           tp.tv_sec, (tp.tv_nsec + 500 * 1000)/(1000 * 1000));
    return 0;
}
