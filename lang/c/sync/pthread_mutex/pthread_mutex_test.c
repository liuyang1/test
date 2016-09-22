#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>

int gVal = 0;
pthread_mutex_t gLock = PTHREAD_MUTEX_INITIALIZER;

#define TIMES 1000000

void *func_nolock(void *p) {
    for (int i = 0; i != TIMES; i++) {
        gVal++;
    }
    return NULL;
}

void *func_lock(void *p) {
    for (int i = 0; i != TIMES; i++) {
        pthread_mutex_lock(&gLock);
        gVal++;
        pthread_mutex_unlock(&gLock);
    }
    return NULL;
}

#include <stdatomic.h>
void *func_atomic(void *p) {
    for (int i = 0; i != TIMES; i++) {
        __sync_fetch_and_add(&gVal, 1);
    }
    return NULL;
}

// ---- test code
#define TIC {                       \
        struct timeval start, stop; \
        gettimeofday(&start, NULL);

#define TOC gettimeofday(&stop, NULL);                                \
    printf("%10lu sec %10luusec\n",                                   \
           stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec); \
    }                                                                 \

int meta_test(void *(*funcPtr)(void *p)) {
    gVal = 0;
    pthread_t t0, t1;

    TIC
    pthread_create(&t0, NULL, funcPtr, NULL);
    pthread_create(&t1, NULL, funcPtr, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    TOC

    bool ret = 2 * TIMES == gVal;
    printf("gVal should be 2 * TIMES = %d ?= %d  %s\n",
           2 * TIMES, gVal, ret ? "success" : "fail");
    return ret;
}

int main() {
    meta_test(func_nolock);

    pthread_mutex_init(&gLock, NULL);
    meta_test(func_lock);
    pthread_mutex_destroy(&gLock);

    meta_test(func_atomic);
    return 0;
}
