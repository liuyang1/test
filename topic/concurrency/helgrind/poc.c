/** use DataRaceDetection of valgrind to check demo program */
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void *worker(void *a){
    int **p = a;
    // sleep(1);
    printf("%p %p\n", p, *p);
    if (*p != NULL) {
        printf("%d\n", **p);
    } else {
        printf("invalid\n");
    }
}
int main() {
    int *a = NULL;
    pthread_t task;
    pthread_create(&task, NULL, worker, &a);

    /** a is uninitialized when creating task
     * This code is logical wrong, but get correct result, due to timing
     */
    // sleep(1);
    a = malloc(sizeof(int));
    *a = 42;

    pthread_join(task, NULL);
    free(a);
    return 0;
}
