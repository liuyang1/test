#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
void *func(void *p) {
    printf("call func(%p)\n", p);
    int cnt = 0;
    while (1) {
        printf("never return in thread\n");
        cnt++;
        sleep(1);
        if (cnt == 5) {
            break;
        }
    }

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, func, NULL);

    sleep(3);
    printf("main thread exit\n");

    return 0;
}
