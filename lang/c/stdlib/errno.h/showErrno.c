#include <errno.h>
#include <string.h>
#include <stdio.h>

int main()
{
    int errnum;
    for(errnum = 1; errnum < 255; errnum++) {
        printf("%3d: %s\n", errnum, strerror(errnum));
    }
    return 0;
}
