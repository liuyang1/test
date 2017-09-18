/**
 * test with C99
 *
 *      gcc usleep.c -std=c99
 */

#include <stdio.h>
// enable useconds_t type
#define __USE_XOPEN
// eanble usleep function
#define __USE_XOPEN_EXTENDED
#include <unistd.h>

int main() {
    useconds_t n = 100;
    usleep(n);
    printf("usleep(%u), exit\n", n);
    return 0;
}
