#include "leet.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
typedef struct {
    int *a;
    int beg;
    int end;
    int cap;
} Queue;

void queueCreate(Queue *q, int maxSize) {
    q->a = malloc(sizeof(int) * maxSize);
    q->cap = maxSize;
    q->beg = 0;
    q->end = 0;
}
void queueDestory(Queue *q) {
    free(q->a);
}

int queueIncMod(Queue *q, int n) {
    return (n + 1) % q->cap;
}
bool queueFull(Queue *q) {
    return queueIncMod(q, q->end) == q->beg;
}
bool queueEmpty(Queue *q) {
    return q->beg == q->end;
}
int queueSize(Queue *q) {
    int v = q->end - q->beg;
    if (v < 0) {
        v += q->cap;
    }
    return v;
}
void queuePush(Queue *q, int elm) {
    if (queueFull(q)) {
        return;
    }
    q->a[q->end] = elm;
    q->end = queueIncMod(q, q->end);
}
void queuePop(Queue *q) {
    if (queueEmpty(q)) {
        return;
    }
    q->a[q->beg] = 0;
    q->beg = queueIncMod(q, q->beg);
}
int queuePeek(Queue *q) {
    if (queueEmpty(q)) {
        return -1;
    }
    return q->a[q->beg];
}

void queueShow(Queue *q) {
    int i;
    printf("beg=%d end=%d cap=%d sz=%d\n",
           q->beg, q->end, q->cap, queueSize(q));
    for (i = 0; i != q->cap; i++) {
        printf("%d,", q->a[i]);
    }
    printf("\n");
}

typedef struct {
    Queue *q;
} Stack;

/* Create a stack */
void stackCreate(Stack *stack, int maxSize) {
    stack->q = malloc(sizeof(Queue));
    queueCreate(stack->q, maxSize + 1);
}

/* Push element x onto stack */
void stackPush(Stack *stack, int element) {
    queuePush(stack->q, element);
}

void queueRecycleTail(Queue *q) {
    int sz = queueSize(q), v, i;
    if (sz <= 1) {
        return;
    }
    for (i = 0; i != sz - 1; i++) {
        v = queuePeek(q);
        queuePop(q);
        queuePush(q, v);
    }
}
/* Removes the element on top of the stack */
void stackPop(Stack *stack) {
    queueRecycleTail(stack->q);
    queuePop(stack->q); // pop last one
}

/* Get the top element */
int stackTop(Stack *stack) {
    queueRecycleTail(stack->q);
    int v = queuePeek(stack->q);
    queuePop(stack->q);
    queuePush(stack->q, v);
    return v;
}

/* Return whether the stack is empty */
bool stackEmpty(Stack *stack) {
    return queueEmpty(stack->q);
}

/* Destroy the stack */
void stackDestroy(Stack *stack) {
    queueDestory(stack->q);
}

int case0() {
    int i, r;
    Stack *stk = malloc(sizeof(Stack));
    stackCreate(stk, 5);
    for (i = 0; i != 5; i++) {
        stackPush(stk, i);
    }
    for (i = 0; i != 5; i++) {
        r = stackTop(stk);
        stackPop(stk);
        printf("pop %d\n", r);
    }
    stackDestroy(stk);
    free(stk);
    return 0;
}

int case1() {
    Stack *stk = malloc(sizeof(Stack));
    stackCreate(stk, 2);
    stackPush(stk, 1);
    stackPush(stk, 2);

    int v = stackTop(stk);
    printf("top %d ?= %d %s\n", v, 2, expect(v == 2));

    stackPop(stk);
    stackPop(stk);

    bool r = stackEmpty(stk);
    printf("empty %s ?= %s\n", SBOOL(r), SBOOL(true));

    stackDestroy(stk);
    return 0;
}

int main() {
    case0();
    case1();
    return 0;
}
