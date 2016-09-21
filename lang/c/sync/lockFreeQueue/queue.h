#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

typedef struct queue_t queue;

queue *open();
void close(queue *q);
void enqueue(queue *q, void *handle);
void *dequeue(queue *q);

#endif
