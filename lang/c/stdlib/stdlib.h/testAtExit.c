#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

// This function is NOT called if a process terminated abnormally because of
// signal.
void onExit(void) {
    printf("When program exit, call this function!\n");
}

int main() {
    printf("program run with pid=%d\n", getpid());
    atexit(onExit);
    printf("register onExit callback function to when atexit\n");

    int i;
    for (i = 0; i != 5; i++) {
        sleep(1);
    }
    printf("program exit\n");
    return 0;
}
