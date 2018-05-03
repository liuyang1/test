/** Solution:
 * Find (i, j, k, l), that a[i] + a[j] + a[k] + a[l] = 0 (0 < i < j < k < l < N)
 * return non-duplicate [(a[i], a[j], a[k], a[l]), ..]
 * convert to:
 * Find (k, l), that a[k] + a[l] = -a[i]-a[j] (k, l C- (j, N), i < j)
 *
 * It's N * N times two-sum problem
 *
 * Complexity: O(N * logN + N * N * N) = O(N^3)
 */
#include "leet.h"

/**
 * Based on this function, we could avoid duplicate result smartly.
 * keep forward until find next different one
 */
int next(int *a, int i, int thresh) {
    do {
        i++;
    } while (a[i - 1] == a[i] && i < thresh);
    return i;
}

int **fourSum(int *nums, int size, int target, int *rsize) {
    if (nums == NULL || size < 4) {
        *rsize = 0;
        return NULL;
    }
    int *a = malloc(sizeof(*a) * size);
    memcpy(a, nums, sizeof(*a) * size);
    qsort(a, size, sizeof(*a), cmpInt);

    int **ret = NULL;
    int i, j, k, l, t, s, rs = 0;
    for (i = 0; i != size - 3; i = next(a, i, size - 3)) {
        for (j = i + 1; j != size - 2; j = next(a, j, size - 2)) {
            t = target - a[i] - a[j];
            for (k = j + 1, l = size - 1; k < l;) {
                s = a[k] + a[l];
                if (s == t) {
                    // build result
                    int *p = malloc(sizeof(int) * 4);
                    p[0] = a[i];
                    p[1] = a[j];
                    p[2] = a[k];
                    p[3] = a[l];
                    rs++;
                    reallocM(&ret, sizeof(int *) * rs);
                    ret[rs - 1] = p;

                    k = next(a, k, l);
                    l--; // a[k] is must different
                         // so even move to same a[l], but will skip it
                } else if (s < t) {
                    k = next(a, k, l);
                } else {
                    l--;
                }
            }
        }
    }
    *rsize = rs;
    free(a);
    return ret;
}

void try(int *nums, int size, int target) {
    int rlen;
    int **a = fourSum(nums, size, target, &rlen);
    showArr2(a, rlen, 4);
    int i;
    for (i = 0; i != rlen; i++) {
        free(a[i]);
    }
    free(a);
}

int main() {
    int a[] = {1, 0, -1, 0, -2, 2};
    try(a, COUNT_OF(a), 0);
    int b[] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4};
    try(b, COUNT_OF(b), 10);
    try(b, COUNT_OF(b), 11);
    try(b, COUNT_OF(b), 12);
    try(b, COUNT_OF(b), 13);
    try(b, COUNT_OF(b), 14);

    return 0;
}
