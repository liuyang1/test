#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

int run = 0;
void *func(void *p) {
    struct timeval tv0, tv1;
    gettimeofday(&tv0, NULL);
    while (run) {
        sleep(10);
    }
    gettimeofday(&tv1, NULL);
    printf("func is ending...dt=%ld\n",
           tv1.tv_sec - tv0.tv_sec);

    return NULL;
}

int main(int argc, char **argv) {
    run = 0;
    pthread_t thread;
    pthread_create(&thread, NULL, func, NULL);
    if (argc == 2 && strcmp(argv[1], "main") == 0) {
        sleep(1);
    }
    run = 1;
    sleep(1);
    run = 0;

    pthread_join(thread, NULL);

    return 0;
}
