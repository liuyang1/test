#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "queue.h"

// #define LOG(...) printf(...)
#define LOG(...)

typedef struct node_t {
    struct node_t *next;
    void *payload;
} node_t;

typedef struct queue_t {
    node_t *head;
    node_t *tail;
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
    return q;
}

void q_close(queue_t *q) {
    // free all node in queue
    while (!q_empty(q)) {
    }
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

    if (q_empty(q)) {
        q->head = n;
    }
    if (q->tail) {
        q->tail->next = n;
    }
    q->tail = n;
}

void *q_dequeue(queue_t *q) {
    // block until queue not q_empty
    while (q_empty(q)) {
        usleep(1);
    }
    if (q->tail == q->head) {
        q->tail = NULL;
    }
    node_t *n = q->head;
    void *handle = n->payload;
    q->head = n->next;
    free(n);
    LOG("%p dequeue %p handle=%p\n", q, n, handle);
    return handle;
}
// API function END
