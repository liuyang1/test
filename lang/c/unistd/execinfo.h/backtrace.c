#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <execinfo.h> // for backtrace

static void showTrace() {
#define ARRAY_SIZE      64
    void *array[ARRAY_SIZE];
    size_t size;
    char **strings;
    char prName[256];

    // get array of return address from stack frame
    size = backtrace(array, ARRAY_SIZE);
    // get symbol from array
    strings = backtrace_symbols(array, size);
    // get name of process
    prctl(PR_GET_NAME, prName, 0, 0, 0);

    printf("%s(pid:%d tid:%zu): backtrace stack %zu level\n",
           prName, getpid(), syscall(SYS_gettid), size);
    for (size_t i = 0; i != size; i++) {
        printf("#%zu: %s\n", i, strings[i]);
    }

    free(strings);
}


int n = 5;
int fib(int i) {
    if (i == n) {
        showTrace();
    }
    if (i <= 1) {
        return 1;
    }
    return fib(i - 1) + fib(i - 2);
}

int main() {
    printf("fib(%d) = %d\n", n, fib(n));
    return 0;
}
