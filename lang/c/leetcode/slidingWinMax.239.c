#include "leet.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
int *maxSlidingWindow(int *nums, int numsSize, int k, int *returnSize) {
    if (k <= 0) {
        *returnSize = 0;
        return NULL;
    }
    int *r = malloc(sizeof(int) * numsSize);
    *returnSize = numsSize + 1 - k;

    memcpy(r, nums, sizeof(int) * numsSize);
    int i, j;
    for (i = 1; i != k; i++) {
        for (j = 0; j != numsSize - i; j++) {
            r[j] = MAX(r[j], r[j + 1]);
        }
    }
    return r;
}

#define CASE(a, k, e) {int sz, *r = maxSlidingWindow(a, COUNT_OF(a), k, &sz); \
                       showArr(r, sz); printf("%s\n", expect(cmpArr(e, COUNT_OF(e), r, sz))); }

int main() {
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 1, ((int[]) {1, 3, -1, -3, 5, 3, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 2, ((int[]) {3, 3, -1, 5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 3, ((int[]) {3, 3, 5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 4, ((int[]) {3, 5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 5, ((int[]) {5, 5, 6, 7}));
    CASE(((int[]) {1, 3, -1, -3, 5, 3, 6, 7}), 6, ((int[]) {5, 6, 7}));
    return 0;
}
