#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"

#define NUM_THREADS     5
void *PrintHello(void *threadid)
{
    long tid = (long)threadid;
    printf("Hello world! from thread #%ld\n", tid);
    pthread_exit(NULL);
}
int main()
{
    pthread_t threads[NUM_THREADS];
    size_t i;
    int ret;
    for (i = 0; i < NUM_THREADS; i++) {
        printf("create thread %ld\n", i);
        ret = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
        if (ret) {
            printf("ERROR: fail at pthread_create %d\n", ret);
            exit(-1);
        }

    }
    pthread_exit(NULL);
    return 0;
}
