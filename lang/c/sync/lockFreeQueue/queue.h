#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

typedef struct queue_t queue;

/**
 * @brief create a queue
 *
 * @return pointer to new queue
 */
queue *q_create();

/**
 * @brief destroy a queue
 * if queue is not empty, it will drain all elements in it.
 *
 * @param q point to queue, not null
 */
void q_destory(queue *q);

/**
 * @brief put one element to queue
 * multi-threading safe.
 *
 * @param q queue,
 * @param handle point to element
 */
void q_enqueue(queue *q, void *handle);

/**
 * @brief get one element from queue
 * multi-threading safe
 * if queue is empty, it will wait until queue had one element to dequeue
 * this blocking use semaphore, so it won't cost too much CPU.
 *
 * @param q queue
 *
 * @return handle, point to element
 */
void *q_dequeue(queue *q);

/**
 * @brief show information in queue
 * expose detail of queue struct
 *
 * @param q queue
 */
void q_show(queue *q);

/**
 * @brief drain all elements in queue
 * multi-threading RISKY
 * this function cannot work in multi-threading environment.
 *
 * @param q queue
 *
 * @return number of drained elements
 */
int q_drain(queue *q);

#endif
