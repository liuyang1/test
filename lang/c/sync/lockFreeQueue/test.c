#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

const char *showBool(bool b) {
    return b ? "true" : "false";
}

#define EXPECT(v, e) printf("%p ?= %p %s\n", v, e, showBool(v == e));
#define VERBOSE

#ifdef VERBOSE
#define CHECK(b, msg) printf("%s %s\n", msg, b ? "success" : "fail");
#else
#define CHECK(b, msg) if (!b) {printf("%s fail\n", msg); }
#endif

bool test_simple() {
#define N 17
    bool ret = true;
    queue *q = q_open();
    long i;
    for (i = 1; i != N; i++) {
        q_enqueue(q, (void *)i);
    }

    q_show(q);

    for (i = 1; i != N; i++) {
        void *p = q_dequeue(q);
        if (p != (void *)i) {
            printf("false\n");
            ret = false;
            break;
        }
    }
    q_close(q);
    return ret;
}

#define PRODUCE_N 10000
void *producer(void *p) {
    queue *q = p;
    for (long i = 1; i != (1 + PRODUCE_N); i++) {
        q_enqueue(q, (void *)i);
        // printf("enqueue %ld\n", i);
    }
    return NULL;
}

void *consumer(void *p) {
    bool *pret = malloc(sizeof(bool));
    *pret = true;
    queue *q = p;
    for (long i = 1; i != (1 + PRODUCE_N); i++) {
        void *n = q_dequeue(q);
        // printf("dequeue %p\n", n);
        if (n != (void *)i) {
            EXPECT(n, (void *)i);
            *pret = false;
            break;
        }
    }
    return pret;
}

pthread_t callFunc_async(void *(*funcPtr)(void *p), queue *q) {
    pthread_t thread;
    pthread_create(&thread, NULL, funcPtr, q);
    return thread;
}

bool test_producer_consumer() {
    queue *q = q_open();
    pthread_t prod = callFunc_async(producer, q);
    pthread_t cons = callFunc_async(consumer, q);

    pthread_join(prod, NULL);
    void *retval;
    pthread_join(cons, &retval);
    bool ret = *(bool *)retval;
    free(retval);

    CHECK(ret, "producer-consumer");
    if (!ret) {
        q_drain(q);
    }
    q_close(q);
    return true;
}

bool test_2producers() {
    queue *q = q_open();
    pthread_t prod0 = callFunc_async(producer, q);
    pthread_t prod1 = callFunc_async(producer, q);
    pthread_join(prod0, NULL);
    pthread_join(prod1, NULL);

    int n = q_drain(q);
    bool ret = n == 2 * PRODUCE_N;
    if (ret != true) {
        printf("produce 2 * %d ?= consume %d\n", PRODUCE_N, n);
    }
    CHECK(ret, "2producers");
    q_close(q);
    return ret;
}

void *berserk(void *p) {
    int *pret = malloc(sizeof(int));
    int n = q_drain(p);
    *pret = n;
    return pret;
}

bool test_2consumers() {
    queue *q = q_open();
    producer(q);

    pthread_t cons0 = callFunc_async(berserk, q);
    pthread_t cons1 = callFunc_async(berserk, q);
    void *retval0, *retval1;
    pthread_join(cons0, &retval0);
    pthread_join(cons1, &retval1);
    int n0 = *(int *)retval0;
    int n1 = *(int *)retval1;
    free(retval0);
    free(retval1);

    bool ret = PRODUCE_N == n0 + n1;
    if (ret != true) {
        printf("produce %d ?= consume %d + %d\n", PRODUCE_N, n0, n1);
    }
    CHECK(ret, "2consumers");
    q_close(q);
    return ret;

}

int main() {
    bool ret = test_simple();
    CHECK(ret, "test_simple case");

    test_producer_consumer();

    test_2producers();

    test_2consumers();
    return 0;
}
