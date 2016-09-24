#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "queue.h"

// #define LOG(fmt, ...) printf("%2lu - " fmt, pthread_self() % 100, __VA_ARGS__)
#define LOG(...)

#define MUTEXLOCK

typedef struct node_t {
    struct node_t *next;
    void *payload;
} node_t;

typedef struct queue_t {
    node_t *head;
    node_t *tail;
    sem_t sem;
#if defined(MUTEXLOCK)
    pthread_mutex_t lock;
#elif defined(LOCKFREE)
#endif
} queue_t;


#if defined(MUTEXLOCK)
#define LOCK(q) pthread_mutex_lock(&q->lock);
#define UNLOCK(q) pthread_mutex_unlock(&q->lock);
#else
#define LOCK(q)
#define UNLOCK(q)
#endif

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

// why not expose `q_empty` as API?
// we cannot do that, as this state is fragile, and change so fast.
// we cannot support this kind of function:
// if (!q_empty(q)) {
//      q_dequeue(q); // when do this, it may empty again, so fail to dequeue.
// }
bool q_empty(queue_t *q) {
    return q->head == NULL;
}

int q_sem_post(queue_t *q) {
    return sem_post(&q->sem);
}

int q_sem_wait(queue_t *q) {
    return sem_wait(&q->sem);
}

// internal function END

// API function
queue_t *q_create() {
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return q;
    }
    q->head = q->tail = NULL;
    sem_init(&q->sem, 0, 0);
#if defined(MUTEXLOCK)
    pthread_mutex_init(&q->lock, NULL);
#elif defined(LOCKFREE)
#endif
    return q;
}

void q_destory(queue_t *q) {
    q_drain(q);
#if defined(MUTEXLOCK)
    pthread_mutex_destroy(&q->lock);
#elif defined(LOCKFREE)
#endif
    sem_destroy(&q->sem);
    free(q);
}

void q_show(queue_t *q) {
    printf("%p show head=%p tail=%p\n", q, q->head, q->tail);
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
    // LOG("%p enqueue %p handle=%p\n", q, n, handle);

    LOCK(q);
    if (q_empty(q)) {
        q->head = n;
    }
    if (q->tail) {
        q->tail->next = n;
    }
    q->tail = n;
    UNLOCK(q);

    q_sem_post(q);
}

void *q_dequeue(queue_t *q) {
    q_sem_wait(q);

    LOCK(q);
    if (q->tail == q->head) {
        q->tail = NULL;
    }
    node_t *n = q->head;
    void *handle = n->payload;
    q->head = n->next;
    UNLOCK(q);

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
