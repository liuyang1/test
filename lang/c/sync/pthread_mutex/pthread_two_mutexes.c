#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>

pthread_mutex_t gLock0 = PTHREAD_MUTEX_INITIALIZER; // statically allocated
pthread_mutex_t gLock1 = PTHREAD_MUTEX_INITIALIZER;

void *func_lock(void *p) {
    for (int i = 0; i != TIMES; i++) {
        pthread_mutex_lock(&gLock0);
        pthread_mutex_unlock(&gLock0);
    }
    return NULL;
}
