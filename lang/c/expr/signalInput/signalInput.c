#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#define BUFLEN  1024

static bool bExit = false;

static void usr_sig_handler(int sig) {
    printf("\nTerminating by Ctrl-C sig=%d\n", sig);
    bExit = true;
}

int main() {
#ifdef USER_DEFINED_HANDLER
    signal(SIGINT, usr_sig_handler);
#endif

    char str[BUFLEN] = {0};
    while (!bExit) {
        char *p = fgets(str, BUFLEN, stdin);
        if (p == NULL) {
            printf("fgets return null, break\n");
            break;
        }
        printf("echo back: %s\n", p);
    }
    return 0;
}
