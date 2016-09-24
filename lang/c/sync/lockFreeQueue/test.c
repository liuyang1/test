#include <stdint.h>
#include <stdio.h>
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

#define N1 100
void *producer(void *p) {
    queue *q = p;
    for (long i = 1; i != N1; i++) {
        q_enqueue(q, (void *)i);
        printf("enqueue %ld\n", i);
    }
    return NULL;
}

void *consumer(void *p) {
    bool ret = true;
    queue *q = p;
    for (long i = 1; i != N1; i++) {
        void *n = q_dequeue(q);
        printf("dequeue %p\n", n);
        if (n != (void *)i) {
            EXPECT(n, (void *)i);
            ret = false;
            break;
        }
    }
    CHECK(ret, "producer-consumer");
    return NULL;
}

pthread_t callFunc_async(void *(*funcPtr)(void *p), queue *q) {
    pthread_t thread;
    pthread_create(&thread, NULL, funcPtr, q);
    return thread;
}

bool test_concurrent() {
    queue *q = q_open();
    pthread_t prod = callFunc_async(producer, q);
    pthread_t cons = callFunc_async(consumer, q);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    q_close(q);
    return true;
}

int main() {
    bool ret = test_simple();
    CHECK(ret, "test_simple case");

    test_concurrent();
    return 0;
}
