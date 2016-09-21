#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

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

pthread_t callFunc_async(void *(*funcPtr)(void *p)) {
    pthread_t thread;
    pthread_create(&thread, NULL, funcPtr, NULL);
    return thread;
}

int test_nolock() {
    gVal = 0;
    pthread_t t0 = callFunc_async(func_nolock);
    pthread_t t1 = callFunc_async(func_nolock);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("gVal should be 2 * TIMES = %d ?= %d\n", 2 * TIMES, gVal);
    return 2 * TIMES == gVal;
}

int test_lock() {
    gVal = 0;

    pthread_t t0 = callFunc_async(func_lock);
    pthread_t t1 = callFunc_async(func_lock);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);


    printf("gVal should be 2 * TIMES = %d ?= %d\n", 2 * TIMES, gVal);
    return 2 * TIMES == gVal;
}

int test_atomic() {
    gVal = 0;
    pthread_t t0 = callFunc_async(func_atomic);
    pthread_t t1 = callFunc_async(func_atomic);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("gVal should be 2 * TIMES = %d ?= %d\n", 2 * TIMES, gVal);
    return 2 * TIMES == gVal;
}

#include <time.h>
#include <sys/time.h>
#define TIC {                       \
        struct timeval start, stop; \
        gettimeofday(&start, NULL);

#define TOC gettimeofday(&stop, NULL);                                \
    printf("%10lu sec %10luusec\n",                                       \
           stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec); \
    }                                                                 \

int main() {
    TIC
    test_nolock();
    TOC

    pthread_mutex_init(&gLock, NULL);
    TIC
    test_lock();
    TOC
    pthread_mutex_init(&gLock, NULL);

    TIC
    test_atomic();
    TOC
    return 0;
}
