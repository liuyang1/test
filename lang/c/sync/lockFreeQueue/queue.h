#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

typedef struct queue_t queue;

queue *q_open();
void q_close(queue *q);
void q_enqueue(queue *q, void *handle);
void *q_dequeue(queue *q);
void q_show(queue *q);

#endif
