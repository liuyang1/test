#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#define Log printf

typedef struct queue_s {
    char *p;
} queue_s;

queue_s *openqueue() {
    queue_s *q = malloc(sizeof(struct queue_s));
    Log("open q=%p\n", q);
    return q;
}

int closequeue(queue_s *q) {
    free(q);
    return 0;
}

int enqueue(queue_s *q, void *buf) {
    Log("q=%p buf=%p\n", q, buf);
    q->p = buf;
    return 0;
}

void *dequeue(queue_s *q) {
    void *p = q->p;
    Log("q=%p buf=%p\n", q, p);
    return p;
}
