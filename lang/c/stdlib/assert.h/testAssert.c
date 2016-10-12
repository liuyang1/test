#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void cleanup() {
    printf("cleanup function is called\n");
}
int main() {
    if (atexit(cleanup) != 0) {
        printf("cannot setting atexit func\n");
    }
    printf("set atexit function to call cleanup\n");

    // assert func will print log, which contain file, line, func, etc.
    // assert func call ABORT().
    // atexit funciton WON'T call.
    assert(0);

    return 0;
}
