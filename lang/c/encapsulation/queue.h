#ifndef QUEUE_H_
#define QUEUE_H_

// XXX
// Reference how to expose interface, but not reveal internal struct.
//
// This definition is legal because struct, union, enumeration tags occpuy a
// name space, that is separate from the space for variable, functions, and
// type names.
typedef struct queue_s queue_t;

// XXX This `void` is IMPORTANT
// - adding this `void`, compiler will consider this function prototype must
// have zero arguments.
// - without this `void`, compiler will consider this function prototype may
// have any numbers of arguments with any type, but won't give any warning with
// any waring level.
queue_t *openqueue(void);
int closequeue(queue_t *q);

int enqueue(queue_t *q, void *buf);
void *dequeue(queue_t *q);

#endif
