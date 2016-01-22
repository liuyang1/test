#include "leet.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
typedef struct {
    int *a;
    int *m;
    int dep;
    int cap;
} MinStack;

void minStackCreate(MinStack *stack, int maxSize) {
    stack->a = malloc(sizeof(int) * maxSize);
    stack->m = malloc(sizeof(int) * maxSize);
    stack->dep = 0;
    stack->cap = maxSize;
}

void minStackPush(MinStack *stack, int element) {
    if (stack->dep == stack->cap) {
        return;
    }
    stack->a[stack->dep] = element;
    if (stack->dep == 0) {
        stack->m[stack->dep] = element;
    } else {
        stack->m[stack->dep] = MIN(stack->m[stack->dep - 1], element);
    }
    stack->dep++;
}

bool minStackEmpty(MinStack *stack) {
    return stack->dep == 0;
}
void minStackPop(MinStack *stack) {
    if(minStackEmpty(stack)) {
        return;
    }
    stack->dep--;
}

int minStackTop(MinStack *stack) {
    if (minStackEmpty(stack)) {
        return -1;
    }
    return stack->a[stack->dep - 1];
}

int minStackGetMin(MinStack *stack) {
    if (minStackEmpty(stack)) {
        return -1;
    }
    return stack->m[stack->dep - 1];
}

void minStackDestroy(MinStack *stack) {
    free(stack->a);
    free(stack->m);
}

void case0() {
    MinStack minstack, *ms = &minstack;
    minStackCreate(ms, 10);

    minStackPush(ms, 4);
    minStackPush(ms, 2);
    minStackPush(ms, 3);
    minStackPush(ms, 1);
    minStackPush(ms, 5);

    while (!minStackEmpty(ms)) {
        printf("%d %d\n", minStackTop(ms), minStackGetMin(ms));
        minStackPop(ms);
    }

    minStackDestroy(ms);
}
int main() {
    case0();
    return 0;
}
