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
#define N 100
    bool ret = true;
    queue *q = open();
    for (long i = 0; i != N; i++) {
        enqueue(q, (void *)i);
    }
    for (long i = 0; i != N; i++) {
        void *p = dequeue(q);
        if (p != (void *)i) {
            ret = false;
        }
    }
    close(q);
    return ret;
}

#define N1 10
void *producer(void *p) {
    queue *q = p;
    for (long i = 0; i != N1; i++) {
        enqueue(q, (void *)i);
        printf("enqueue %ld\n", i);
    }
    return NULL;
}

void *consumer(void *p) {
    bool ret = true;
    queue *q = p;
    for (long i = 0; i != N1; i++) {
        void *n = dequeue(q);
        printf("dequeue %p\n", n);
        // if (p != (void *)i) {
        //     EXPECT(p, (void *)i);
        //     ret = false;
        //     break;
        // }
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
    queue *q = open();
    pthread_t prod = callFunc_async(producer, q);
    pthread_t cons = callFunc_async(consumer, q);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    close(q);
    return true;
}

int main() {
    bool ret = test_simple();
    CHECK(ret, "test_simple case");

    test_concurrent();
    return 0;
}
