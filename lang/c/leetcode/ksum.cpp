/**
 * Leetcode have 2-sum, 3-sum, and 4-sum problems. We could implement one k-sum solution.
 * COmplexity: O(n ^ (k - 1))
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int next(int *nums, int size, int k) {
    do {
        k++;
    } while (nums[k] == nums[k - 1] && k < size);
    return k;
}

int **ksum(int *nums, int size, int k, int target, int *rsize) {
    // sorted, assert k >= 2
    int **ret = NULL;
    int sz = 0;
    if (k > 2) {
        int i;
        for (i = 0; i != size - k;) {
            // recursion to k - 1 problem
            int rlen;
            int **r = ksum(nums + i + 1, size - i - 1, k - 1, target - nums[i], &rlen);

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

            i = next(nums, size - k, i);
        }
    } else if (k == 2) {
        int b, e, s;
        for (b = 0, e = size - 1; b < e;) {
            s = nums[b] + nums[e];
            if (s < target) {
                b = next(nums, e, b);
            } else if (s > target)   {
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
    }
    *rsize = sz;
    return ret;
}

int main() {
    int a[] = {-10, -4, -3, -2, -1, -1, 0, 0, 1, 2, 2, 3, 3, 4, 5};
    int k = 5;
    int rlen;
    int **r = ksum(a, sizeof(a) / sizeof(a[0]), k, 0, &rlen);

    int i, j;
    for (i = 0; i != rlen; i++) {
        for (j = 0; j != k; j++) {
            printf("%d ", r[i][j]);
        }
        printf("\n");
    }
    return 0;
}
