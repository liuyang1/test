#include "leet.h"
// one simple limit-capcity queue
// implement based on array with ring index
typedef struct {
    int *a;
    int beg;
    int end;
    int cap;
} Queue;

Queue *q_create(int capcity) {
    Queue *q =  malloc(sizeof(Queue));
    q->cap = capcity + 1;
    q->a = malloc(sizeof(int) * q->cap);
    q->beg = q->end = 0;
    return q;
}

void q_show(Queue *q) {
    printf("q=%p a=%p [%d, %d) / %d\t", q, q->a, q->beg, q->end, q->cap);
    showArr(q->a, q->cap);
}

int q_inc(Queue *q, int idx) {
    return (idx + 1) % (q->cap);
}

int q_dec(Queue *q, int idx) {
    return (idx + q->cap - 1) % (q->cap);
}

bool q_isempty(Queue *q) {
    return q->beg == q->end;
}

bool q_isfull(Queue *q) {
    return q_inc(q, q->end) == q->beg;
}

bool q_pushback(Queue *q, int v) {
    if (q_isfull(q)) {
        return false;
    }
    q->a[q->end] = v;
    q->end = q_inc(q, q->end);
    return true;
}

#define ASSERT_EMPTY(q) { if (q_isempty(q)) { \
                              LOG("%s", "should not empty\n"); return -1; } }
int q_peekback(Queue *q) {
    ASSERT_EMPTY(q);
    return q->a[q_dec(q, q->end)];
}

int q_peekfront(Queue *q) {
    ASSERT_EMPTY(q);
    return q->a[q->beg];
}

int q_popfront(Queue *q) {
    ASSERT_EMPTY(q);
    int r = q_peekfront(q);
    q->a[q->beg] = 0;
    q->beg = q_inc(q, q->beg);
    return r;
}

int q_popback(Queue *q) {
    ASSERT_EMPTY(q);
    int r = q_peekback(q);
    q->a[q->end] = 0;
    q->end = q_dec(q, q->end);
    return r;
}

void q_destory(Queue *q) {
    if (q) {
        if (q->a) {
            free(q->a);
        }
        free(q);
    }
}

// ---- end of queue

int *maxSlidingWindow(int *nums, int numsSize, int k, int *returnSize) {
    if (k <= 0) {
        *returnSize = 0;
        return NULL;
    }
    if (k > numsSize) {
        k = numsSize;
    }
    *returnSize = numsSize + 1 - k;
    int *r = malloc(sizeof(int) * (*returnSize));
    if (k == 1) {
        memcpy(r, nums, sizeof(int) * (*returnSize));
        return r;
    }

    Queue *q = q_create(k);
    int i;
    for (i = 0; i != k; i++) {
        while (!q_isempty(q) && nums[i] >= nums[q_peekback(q)]) {
            q_popback(q);
        }
        q_pushback(q, i);
    }
    for (; i != numsSize; i++) {
        r[i - k] = nums[q_peekfront(q)];

        while (!q_isempty(q) && q_peekfront(q) <= i - k) {
            q_popfront(q);
        }
        while (!q_isempty(q) && nums[i] >= nums[q_peekback(q)]) {
            q_popback(q);
        }
        q_pushback(q, i);
    }
    r[i - k] = nums[q_peekfront(q)];
    q_destory(q);

    return r;
}

#define CASE(a, k, e) {int sz, *r = maxSlidingWindow(a, COUNT_OF(a), k, &sz); \
                       showArr(r, sz);                                        \
                       printf("%s\n", expect(cmpArr(e, COUNT_OF(e), r, sz))); \
                       if (r) { free(r); } }

int main() {
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 1, ((int[]) {1, 3, -1, -3, 5, 3, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 2, ((int[]) {3, 3, -1, 5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 3, ((int[]) {3, 3, 5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 4, ((int[]) {3, 5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 5, ((int[]) {5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 6, ((int[]) {5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 7, ((int[]) {6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 8, ((int[]) {7}));
    return 0;
}
