#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void chkdie(bool b, char *msg) {
    if (b) {
#define LEN     256
        char s[LEN] = {0};
        snprintf(s, LEN, "fail at %s", msg);
        perror(msg);
        exit(-1);
    }
}

int main() {
    struct timespec ts;
    sem_t sem;
    int ret = sem_init(&sem, 0, 0);
    chkdie(ret == -1, "sem_int");
