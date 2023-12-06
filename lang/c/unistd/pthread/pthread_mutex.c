#include <stdio.h>
#include "pthread.h"

int main() {
    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    printf("start test\n");
    int r = pthread_mutex_lock(&m);
    printf("enter mutex r=%d\n", r);
    r = pthread_mutex_unlock(&m);
    printf("leave mutex r=%d\n", r);
    return 0;
}
