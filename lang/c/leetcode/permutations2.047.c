#include "leet.h"
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int *a, int n) {
    int i, t;
    for (i = 0; i != n / 2; i++) {
        t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}

bool nextPermutation(int *nums, int numsSize) {
    int i, j;
    for (i = numsSize - 1; i > 0; i--) {
        if (nums[i] > nums[i - 1]) {
            break;
        }
    }
    for (j = numsSize - 1; j > 0; j--) {
        if (nums[j] > nums[i - 1]) {
            break;
        }
    }
    if (i == 0) {
        return false;
    }
    swap(nums + i - 1, nums + j);
    reverse(nums + i, numsSize - i);
    return true;
}

int factor(int n) {
    int r = 1, i;
    for (i = n; i != 1; i--) {
        r *= i;
    }
    return r;
}

int permNum(int *nums, int numsSize) {
    if (numsSize <= 0) {
        return 0;
    }
    int r = factor(numsSize), n, cnt, i;
    for (i = 1, cnt = 1, n = nums[0]; i != numsSize; i++) {
        if (nums[i] == n) {
            cnt++;
        } else {
            r /= factor(cnt);
            n = nums[i];
            cnt = 1;
        }
    }
    r /= factor(cnt);
    return r;
}

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int **permuteUnique(int *nums, int numsSize, int *returnSize) {
    int *n = malloc(sizeof(int) * numsSize);
    memcpy(n, nums, sizeof(int) * numsSize);
    qsort(n, numsSize, sizeof(int), cmpInt);

    int sz = permNum(n, numsSize);
    int **r = malloc(sizeof(int *) * sz);

    int i, j;
    for (i = 0; i != sz; i++) {
        r[i] = malloc(sizeof(int) * numsSize);
        for (j = 0; j != numsSize; j++) {
            r[i][j] = n[j];
        }
        nextPermutation(n, numsSize);
    }

    free(n);
    *returnSize = sz;
    return r;
}

#define CASE(a) { int rsz, **r = permuteUnique(a, COUNT_OF(a), &rsz); \
                  printf("returnSize=%d\n", rsz); \
                  int i; \
                  for (i = 0; i != rsz; i++) { \
                      showArr(r[i], COUNT_OF(a)); \
                      free(r[i]); \
                  } \
                  if (r) { free(r); } }
int main() {
    CASE(((int[]) {3, 2, 1}));
    CASE(((int[]) {3, 1, 1}));
    CASE(((int[]) {3, 3, 1, 1}));
    CASE(((int[]) {3, 4, 2, 1}));
    CASE(((int[]) {3, 3, 0, 3}));
    return 0;
}
