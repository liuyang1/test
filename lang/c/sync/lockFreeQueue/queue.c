#include <stdlib.h>
#include "queue.h"

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
// internal function END

// API function
queue_t *open() {
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return q;
    }
    q->head = q->tail = NULL;
    return q;
}

void close(queue_t *q) {
    // free all node in queue
    while (dequeue(q) != NULL) {
    }
    free(q);
}

void enqueue(queue_t *q, void *handle) {
    node_t *n = node_create(handle);

    if (q->head == NULL) {
        q->head = n;
    }
    if (q->tail == NULL) {
        q->tail = n;
    } else {
        q->tail->next = n;
        q->tail = n;
    }
}

void *dequeue(queue_t *q) {
    if (q->head == NULL) {
        return NULL;
    }
    if (q->tail == q->head) {
        q->tail = NULL;
    }
    void *handle = q->head->payload;
    q->head = q->head->next;
    return handle;
}
// API function END
