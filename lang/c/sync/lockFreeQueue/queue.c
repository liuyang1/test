#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include "queue.h"

// #define LOG(fmt, ...) printf("tid=%02lu - " fmt, pthread_self() % 100, __VA_ARGS__)
#define LOG(...)

// have to define this value as macro instead of enum,
// as preprocessor handle all these.
#define NOLOCK      0
#define MUTEXLOCK   1
#define LOCKFREE    2
#define LOCKTYPE    LOCKFREE

#define CAS __sync_bool_compare_and_swap

typedef struct node_t {
    struct node_t *next;
    void *payload;
} node_t;

typedef struct queue_t {
    node_t *head;
    node_t *tail;
    sem_t sem;
#if (LOCKTYPE == MUTEXLOCK)
    pthread_mutex_t lock;
#endif
} queue_t;


#if LOCKTYPE == NOLOCK || LOCKTYPE == LOCKFREE
#define LOCK(q)
#define UNLOCK(q)
#elif (LOCKTYPE == MUTEXLOCK)
#define LOCK(q) pthread_mutex_lock(&q->lock);
#define UNLOCK(q) pthread_mutex_unlock(&q->lock);
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
    return q->head->next == NULL;
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
    q->head = q->tail = node_create(0); // create dummy header
    // why need dummy header?
    // with dummy header, we could independent operate head and tail pointer.
    // without it, we have to setting head, and tail SAME time
    //      when enqueue first one or deuqueue last sone
    // so enqueue and dequeue must be mutex, this cost too much.
    sem_init(&q->sem, 0, 0);
#if (LOCKTYPE == MUTEXLOCK)
    pthread_mutex_init(&q->lock, NULL);
#endif
    return q;
}

void q_destory(queue_t *q) {
    assert(q != NULL);
    assert(q->head != NULL);
    q_drain(q);
#if (LOCKTYPE == MUTEXLOCK)
    pthread_mutex_destroy(&q->lock);
#endif
    sem_destroy(&q->sem);
    node_destory(q->head);
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
    LOG("%p enqueue %p handle=%p\n", q, n, handle);

#if LOCKTYPE != LOCKFREE
    LOCK(q);
    q->tail->next = n;
    q->tail = n;
    UNLOCK(q);
#elif LOCKTYPE == LOCKFREE
    CAS(&q->head, NULL, n);
    node_t *tail;
    // if tail->next != NULL, then loop again
    // if tail->next == NULL, then set it to q
    do {
        tail = q->tail;
    } while (tail != NULL && CAS(&tail->next, NULL, n) != true);

    // if q->tail == tail, then q->tail <= node
    // if not, then do nothing (tail node as updated)
    CAS(&q->tail, tail, n);
#endif

    q_sem_post(q);
}

void *q_dequeue(queue_t *q) {
    q_sem_wait(q);
    assert(q->head != NULL);
    assert(q->tail != NULL);

    node_t *head;
#if LOCKTYPE != LOCKFREE
    LOCK(q);
    head = q->head;
    q->head = head->next;
    UNLOCK(q);
#elif LOCKTYPE == LOCKFREE
    //      DUMMY -> FIRST -> 2nd
    //      Q.HEAD
    // after CAS operator
    //      DUMMY -> FIRST -> 2nd
    //               Q.HEAD
    // next time, FIRST will work as new DUMMY node.
    do {
        head = q->head;
    } while (CAS(&q->head, head, head->next) != true);
    // with moving DUMMY style, we only move Q.HEAD.
    // so enqueue and dequeue have no relationship.
#endif
    // this local HEAD point to DUMMY, fetch its next to get handle.
    node_t *n = head->next;
    void *handle = n->payload;

    // free DUMMY point now.
    free(head);
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
