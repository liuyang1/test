#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "queue.h"

// #define LOG(...) printf(...)
#define LOG(...)

#define LOCK

typedef struct node_t {
    struct node_t *next;
    void *payload;
} node_t;

typedef struct queue_t {
    node_t *head;
    node_t *tail;
#if defined(LOCK)
    pthread_mutex_t lock;
#elif defined(LOCKFREE)
#endif
} queue_t;

// internal function
node_t *node_create(void *handle) {
    node_t *n = malloc(sizeof(node_t));
    if (n == NULL) {
        return n;
    }
    n->payload = handle;
    n->next = NULL;
    return n;
}

void node_destory(node_t *n) {
    free(n);
}

bool q_empty(queue_t *q) {
    return q->head == NULL;
}

// internal function END

// API function
queue_t *q_open() {
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return q;
    }
    q->head = q->tail = NULL;
#if defined(LOCK)
    pthread_mutex_init(&q->lock, NULL);
#elif defined(LOCKFREE)
#endif
    return q;
}

void q_close(queue_t *q) {
    // free all node in queue
    while (!q_empty(q)) {
    }
#if defined(LOCK)
    pthread_mutex_destroy(&q->lock);
#elif defined(LOCKFREE)
#endif
    free(q);
}

void q_show(queue_t *q) {
    printf("%p show head=%p tai=%p\n", q, q->head, q->tail);
    int i;
    node_t *n;
    for (n = q->head, i = 0; n != NULL; n = n->next, i++) {
        printf("%p(%p) ", n, n->payload);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void q_enqueue(queue_t *q, void *handle) {
    node_t *n = node_create(handle);
    LOG("%p enqueue %p handle=%p\n", q, n, handle);

#if defined(LOCK)
    pthread_mutex_lock(&q->lock);
#elif defined(LOCKFREE)
#endif

    if (q_empty(q)) {
        q->head = n;
    }
    if (q->tail) {
        q->tail->next = n;
    }
    q->tail = n;

#if defined(LOCK)
    pthread_mutex_unlock(&q->lock);
#elif defined(LOCKFREE)
#endif
}

void *q_dequeue(queue_t *q) {
    // block until queue not q_empty
    while (q_empty(q)) {
        usleep(1);
    }

#if defined(LOCK)
    pthread_mutex_lock(&q->lock);
#elif defined(LOCKFREE)
#endif

    if (q->tail == q->head) {
        q->tail = NULL;
    }
    node_t *n = q->head;
    void *handle = n->payload;
    q->head = n->next;

#if defined(LOCK)
    pthread_mutex_unlock(&q->lock);
#elif defined(LOCKFREE)
#endif
    free(n);
    LOG("%p dequeue %p handle=%p\n", q, n, handle);
    return handle;
}

int q_drain(queue_t *q) {
    int i;
    for (i = 0; !q_empty(q); i++) {
        q_dequeue(q);
    }
    return i;
}

// API function END
