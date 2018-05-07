/**
 * Leetcode have 2-sum, 3-sum, and 4-sum problems
 * We could implement one k-sum solution
 *
 * Solution:
 * Find (i, .., k, l), that a[i] + .., + a[k] + a[l] = 0 (0 < i < .., < k < l < N)
 * return non-duplicate [(a[i], .., a[k], a[l]), ..]
 * convert to:
 * Find (k, l), that a[k] + a[l] = -a[i]-.., (k, l C- (j, N), i < j)
 *
 * It's N ^ (k - 2) times two-sum problem
 *
 * Bias-optimization
 * - if all potential combination is too big, so we don't need continue to check it
 *
 * Left-Right-optimzation
 * - try to loop from left, then right, This is USELESS
 *
 * Complexity: O(n ^ (k - 1))
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int next(int *nums, int size, int k) {
    do {
        k++;
    } while (nums[k] == nums[k - 1] && k < size);
    return k;
}

int **ksum(int *nums, int size, int k, int target, int *rsize, bool *pbias) {
    // sorted, assert k >= 2
    int **ret = NULL;
    int sz = 0;
    if (pbias != NULL) {
        *pbias = false;
    }
    if (k > 2) {
        int i;
        for (i = 0; i != size - k + 1;) {
            // recursion to k - 1 problem
            int rlen;
            bool bias;
            int **r = ksum(nums + i + 1, size - i - 1, k - 1, target - nums[i], &rlen, &bias);
            if (bias) {
                break;
            }

            // integrate with low level answer
            if (rlen != 0) {
                ret = (int **)realloc(ret, sizeof(int *) * (sz + rlen));
                int j;
                for (j = 0; j != rlen; j++) {
                    int *p = (int *)malloc(sizeof(int) * k);
                    p[0] = nums[i];
                    memcpy(p + 1, r[j], sizeof(int) * (k - 1));
                    ret[sz + j] = p;
                    free(r[j]);
                }
                free(r);
                sz += rlen;
            }

            i = next(nums, size - k + 1, i);
        }
        if (pbias != NULL && sz == 0) {
            *pbias = i != size - k +1;
        }
    } else if (k == 2) {
        int b, e, s;
        for (b = 0, e = size - 1; b < e;) {
            s = nums[b] + nums[e];
            if (s < target) {
                b = next(nums, e, b);
            } else if (s > target) {
                e--;
            } else {
                ret = (int **)realloc(ret, sizeof(int *) * (sz + 1));
                int *ret0 = (int *)malloc(sizeof(int) * 2);
                ret0[0] = nums[b];
                ret0[1] = nums[e];
                ret[sz] = ret0;
                sz++;

                b = next(nums, e, b);
                e--;
            }
        }
        if (pbias != NULL && sz == 0) {
            *pbias = b == 0;
        }
    }
    *rsize = sz;
    return ret;
}

#include "leet.h"

int main() {
    int a[] = {-10, -4, -3, -2, -1, -1, 0, 0, 1, 2, 2, 3, 3, 4, 5};
    int k = 5;
    int rlen;
    int **r = ksum(a, sizeof(a) / sizeof(a[0]), k, 0, &rlen, NULL);

    showArr2(r, rlen, k);
    printf("rlen=%d\n", rlen);
    int i;
    // duplicate checking
    int j, m, dupcnt = 0;
    for (i = 0; i != rlen; i++) {
        for (j = i + 1; j != rlen; j++) {
            bool dup = true;
            for (m = 0; m != k; m++) {
                if (r[i][m] != r[j][m]) {
                    dup = false;
                    break;
                }
            }
            if (dup) {
                printf("%d:\t", i);
                showArr(r[i], k);
                printf("%d:\t", j);
                showArr(r[j], k);
                dupcnt++;
            }
        }
    }
    printf("dupcnt=%d\n", dupcnt);

    for (i = 0; i != rlen; i++) {
        free(r[i]);
    }
    free(r);
    return 0;
}
