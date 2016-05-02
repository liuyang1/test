#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

int gAnswer = 0;
bool gAnswerReady = false;

void *task0(void *args) {
    gAnswerReady = true;
    gAnswer = 42;
    return NULL;
}

void *task1(void *args) {
    if (gAnswerReady) {
        printf("The meaning of life is : %d\n", gAnswer);
    } else {
        printf("I don't know the answer.\n");
    }
    return NULL;
}

int main() {
    pthread_t t0, t1;
    pthread_create(&t0, NULL, task0, NULL);
    pthread_create(&t1, NULL, task1, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    return 0;
}
