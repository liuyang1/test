#include <stdio.h>
#include <stdlib.h>

int nomain(void)
{
    printf("test nomain\n");
    exit(0);
    // not return
    // as no CRT init, so not could return.
    // CRT -> _start -> main
    // return of main -> exit
    // so now, need manually exit
    return 0;
}
