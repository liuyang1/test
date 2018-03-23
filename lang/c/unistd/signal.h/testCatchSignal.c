#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <signal.h>

typedef void (*sighandler_t)(int);

void trap(int sig) {
    printf("trap sig=%d\n", sig);
}

int main() {
    int i;
    for (i = 1; i != 30; i++) {
        sighandler_t sighdl = signal(i, trap);
        if (sighdl == SIG_ERR) {
            printf("reg on signal=%d with trap func FAIL: errno=%d %s\n", i, errno, strerror(errno));
        }
        // printf("reg on signal=%d with trap func\n", i);
    }
    return 0;
}
