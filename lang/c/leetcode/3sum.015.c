#include "leet.h"
#include <stdio.h>
#include <stdlib.h>

int cmpInt(const void *n0, const void *n1) {
    return (*(int *)n0) - (*(int *)n1);
}

// compare triplet
// 1: GT; 0: EQ; -1: LT
int cmpTri(const int *p0, const int *p1) {
    int i, diff;
    for (i = 0; i != 3; i++) {
        diff = p0[i] - p1[i];
        if (diff != 0) {
            return diff;
        }
    }
    return 0;
}

// binary search duplicate triplet
// 1: find; 0: not find
int findDup_bin(int **a, int size, int *p) {
    int bgn, end, mid;
    for (bgn = 0, end = size - 1; bgn <= end;) {
        mid = (bgn + end) / 2;
        int c = cmpTri(a[mid], p);
        if (c == 0) {
            return 1;
        } else if (c < 0) {
            bgn = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return 0;
}

// search duplicate triplet from tail
// for user case of this problem, a is sorted, and p >= a[-1],
// just check if p == a[-1] is enough
int findDup_tail(int **a, int size, int *p) {
    int i;
    for (i = size - 1; i >= 0; i--) {
        int c = cmpTri(a[i], p);
        if (c == 0) {
            return 1;
        } else if (c < 0) {
            break;
        } else if (c > 0) {
            // should not here
            continue;
        }
    }
    return 0;
}

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 * This function affect order in nums.
 */
int **threeSum(int *nums, int numsSize, int *returnSize) {
    if (returnSize == NULL) {
        return NULL;
    }
    qsort(nums, numsSize, sizeof(int), cmpInt);
    int **ret = NULL;
    int size = 0, i;
    for (i = 0; i < numsSize - 2; i++) {
        int bgn, end;
        for (bgn = i + 1, end = numsSize - 1; bgn < end;) {
            int sum = nums[i] + nums[bgn] + nums[end];
            if (sum == 0) {
                int *p = malloc(sizeof(int) * 3);
                p[0] = nums[i];
                p[1] = nums[bgn];
                p[2] = nums[end];
                if (findDup_tail(ret, size, p)) {
                    free(p);
                } else {
                    size++;
                    reallocM(&ret, sizeof(int *) * size);
                    ret[size - 1] = p;
                }
                // continue to search next pair
                // nums[bgn] + nums[end] = -nums[i];
                bgn++;
                end--;
            } else if (sum > 0) {
                end--;
            } else {
                bgn++;
            }
        }
    }
    *returnSize = size;
    return ret;
}

int test(int *nums, int numsSize) {
    int size;
    int **ret = threeSum(nums, numsSize, &size);

    printf(">>> ");
    showArr(nums, numsSize);
    int i;
    for (i = 0; i != size; i++) {
        showArr(ret[i], 3);
    }
    printf("\n");

    for (i = 0; i != size; i++) {
        free(ret[i]);
    }
    free(ret);
    return 0;
}

int main() {
    int a[] = {-1, 0, 1, 2, -1, 4};
    test(a, sizeof(a) / sizeof(int));
    int b[] = {0, 0, 0, 0};
    test(b, sizeof(b) / sizeof(int));
    int c[] = {1, -1, -1, 0};
    test(c, sizeof(c) / sizeof(int));
    int d[] = {-2, 0, 1, 1, 2};
    test(d, sizeof(d) / sizeof(int));
    return 0;
}
