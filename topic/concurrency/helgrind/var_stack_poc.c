/** use DataRaceDetection of valgrind to check demo program */
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void *worker(void *a){
    int *p = a;
    printf("%p %d\n", p, *p);
    return NULL;
}

pthread_t task;
int subfn() {
    /** a is var in stack, It's risky to pass var in stack to async-task
     * This code is logical wrong, but get correct result, due to timing
     */
    int a = 42;
    pthread_create(&task, NULL, worker, &a);
    return 0;
}

int subfn1(int x, int y) {
    printf("arg addr %p %p\n", &x, &y);
    return x + y;
}

int main() {

    int r;
    subfn();
    r = subfn1(21, 21);
    printf("r=%d\n", r);

    pthread_join(task, NULL);
    return 0;
}
