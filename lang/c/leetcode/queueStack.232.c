#include "leet.h"
typedef int StackElmType;
typedef struct {
    StackElmType *a;
    unsigned int dep;
    unsigned int cap;
} Stack;

void stackCreate(Stack *stk, int maxSize) {
    stk->a = malloc(sizeof(StackElmType) * maxSize);
    stk->cap = maxSize;
    stk->dep = 0;
}

void stackDestory(Stack *stk) {
    free(stk->a);
}

bool stackEmpty(Stack *stk) {
    return stk->dep == 0;
}
bool stackFull(Stack *stk) {
    return stk->dep == stk->cap;
}

void stackPush(Stack *stk, int elm) {
    if (stackFull(stk)) {
        return;
    }
    stk->a[stk->dep] = elm;
    stk->dep++;
}
void stackPop(Stack *stk) {
    if (stackEmpty(stk)) {
        return;
    }
    stk->dep--;
}
int stackPeek(Stack *stk) {
    if (stackEmpty(stk)) {
        return NIL;
    }
    return stk->a[stk->dep - 1];
}

void stackExchange(Stack *src, Stack *dst) {
    int elm;
    while (!stackEmpty(src)) {
        elm = stackPeek(src);
        stackPush(dst, elm);
        stackPop(src);
    }
}

typedef struct {
    Stack *in;
    Stack *out;
} Queue;

/* Create a queue */
void queueCreate(Queue *queue, int maxSize) {
    queue->in = malloc(sizeof(Stack));
    queue->out = malloc(sizeof(Stack));
    stackCreate(queue->in, maxSize);
    stackCreate(queue->out, maxSize);
}

/* Push element x to the back of queue */
void queuePush(Queue *queue, int element) {
    if (stackEmpty(queue->in)) {
        stackExchange(queue->out, queue->in);
    }
    stackPush(queue->in, element);
}

/* Removes the element from front of queue */
void queuePop(Queue *queue) {
    if (stackEmpty(queue->out)) {
        stackExchange(queue->in, queue->out);
    }
    stackPop(queue->out);
}

/* Get the front element */
int queuePeek(Queue *queue) {
    if (stackEmpty(queue->out)) {
        stackExchange(queue->in, queue->out);
    }
    stackPeek(queue->out);
}

/* Return whether the queue is empty */
bool queueEmpty(Queue *queue) {
    return stackEmpty(queue->in) && stackEmpty(queue->out);
}

/* Destroy the queue */
void queueDestroy(Queue *queue) {
    stackDestory(queue->in);
    stackDestory(queue->out);
}

int case0() {
    int i, r;
    Queue q, *pq = &q;
    queueCreate(pq, 10);
    for (i = 0; i < 5; i++) {
        queuePush(pq, i);
    }
    for (i = 0; i < 5; i++) {
        r = queuePeek(pq);
        printf("peek %d\n", r);
        queuePop(pq);
    }
    queueDestroy(pq);
    return 0;
}

int case1() {
    int i, r;
    Queue q, *pq = &q;
    queueCreate(pq, 10);
    for (i = 0; i < 5; i++) {
        queuePush(pq, i);
        r = queuePeek(pq);
        printf("peek %d\n", r);
        queuePop(pq);
    }
    queueDestroy(pq);
    return 0;
}
int main() {
    case0();
    case1();
    return 0;
}
