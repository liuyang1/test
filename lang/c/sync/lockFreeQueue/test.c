#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "queue.h"

#define VERBOSE
#ifdef VERBOSE
#define CHECK(b, msg) printf("%-20s %s\n", msg, b ? "success" : "fail");
#define EXPECT(v, e) printf("%p ?= %p %s\n", v, e, v == e ? "true" : "false");
#else
#define CHECK(b, msg) if (!b) {printf("%s fail\n", msg); }
#define EXPECT(v, e) if (v != e) {printf("%p ?= %p false\n", v, e); }
#endif

#define PRODUCE_N 10000

/**
 * @brief produce 1..PRODUCE_N elements to queue
 * it is one kind of worker.
 *
 * @param p queue
 *
 * @return NULL
 */
void *producer(void *p) {
    queue *q = p;
    for (long i = 1; i != (1 + PRODUCE_N); i++) {
        q_enqueue(q, (void *)i);
    }
    return NULL;
}

/**
 * @brief consume 1..PRODUCE_N elements from queue
 * if product are out of order or miss any one product, it return false.
 * It will wait until get PRODUCE_N elements.
 * It is one kind of worker.
 *
 * @param p queue
 *
 * @return success or fail
 */
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

/**
 * @brief consume PRODUCE_N / 2 element, then quit.
 * It is one kind of worker.
 *
 * @param p queue
 *
 * @return number of element
 */
void *berserker(void *p) {
    queue *q = p;
    int *pret = malloc(sizeof(int));
    int i;
    // when to exit dequeue process??
    // consumer don't know how many product was produced.
    // so cannot write dead loop function
    for (i = 0; i != PRODUCE_N / 2; i++) {
        q_dequeue(q);
    }
    *pret = i;
    return pret;
}

/**
 * @brief call test thread with queue as parameter
 *
 * @param funcPtr test routine
 * @param q queue
 *
 * @return thread_t
 */
pthread_t start_workder(void *(*funcPtr)(void *p), queue *q) {
    pthread_t thread;
    pthread_create(&thread, NULL, funcPtr, q);
    return thread;
}

/**
 * @brief simple test case, just enqueue elements, and dequeue them.
 * Mono-threading cases.
 *
 * @return success of fail
 */
bool test_simple() {
#define N 17
    bool ret = true;
    queue *q = q_create();
    long i;
    for (i = 1; i != N; i++) {
        q_enqueue(q, (void *)i);
    }

    // q_show(q);

    for (i = 1; i != N; i++) {
        void *p = q_dequeue(q);
        if (p != (void *)i) {
            printf("%p != %p fail\n", p, (void *)i);
            ret = false;
            break;
        }
    }
    q_destory(q);
    CHECK(ret, "test_simple case");
    return ret;
}

/**
 * @brief producer-consumer test case
 * producer and consumer are working as two threads.
 *
 * @return return from consumer routine
 */
bool test_producer_consumer() {
    queue *q = q_create();
    pthread_t prod = start_workder(producer, q);
    pthread_t cons = start_workder(consumer, q);

    pthread_join(prod, NULL);
    void *retval;
    pthread_join(cons, &retval);
    bool ret = *(bool *)retval;
    free(retval);

    CHECK(ret, "producer-consumer");
    if (!ret) {
        q_drain(q);
    }
    q_destory(q);
    return true;
}

/**
 * @brief two producers produce product same time.
 *
 * @return
 * if number of elements in queue is equal to produce elements, return success.
 * Else fail
 */
bool test_2producers() {
    queue *q = q_create();
    pthread_t prod0 = start_workder(producer, q);
    pthread_t prod1 = start_workder(producer, q);
    pthread_join(prod0, NULL);
    pthread_join(prod1, NULL);

    int n = q_drain(q);
    bool ret = n == 2 * PRODUCE_N;
    if (ret != true) {
        printf("produce 2 * %d ?= consume %d\n", PRODUCE_N, n);
    }
    CHECK(ret, "2producers");
    q_destory(q);
    return ret;
}

/**
 * @brief start two berserker to consume queue same time.
 *
 * @return if every berserker get half of PRODUCE_N / 2 element, then success,
 * else fail.
 */
bool test_2consumers() {
    queue *q = q_create();
    producer(q);

    pthread_t cons0 = start_workder(berserker, q);
    pthread_t cons1 = start_workder(berserker, q);
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
    q_destory(q);
    return ret;
}

#define EXPECT_PTR(a_, b_) {                   \
        void *a = (void *)a_, *b = (void *)b_; \
        if (a != b) {                          \
            printf("%p != %p fail\n", a, b);   \
            return false;                      \
        }                                      \
}
bool test_interleave() {
    bool ret = true;
    queue *q = q_create();
    void *handle;

    q_enqueue(q, (void *)1);
    handle = q_dequeue(q);
    EXPECT_PTR(handle, 1);

    q_enqueue(q, (void *)2);
    handle = q_dequeue(q);
    EXPECT_PTR(handle, 2);

    CHECK(ret, __FUNCTION__);
    return ret;
}

#define TIC {                       \
        struct timeval start, stop; \
        gettimeofday(&start, NULL);

#define TOC gettimeofday(&stop, NULL);                                \
    printf("%10lu sec %10luusec\n",                                   \
           stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec); \
    }

int main() {
    TIC
    test_simple();
    test_interleave();
    test_2producers();
    test_2consumers();
    test_producer_consumer();
    TOC
    return 0;
}
