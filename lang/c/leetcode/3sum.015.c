/** Solution:
 * Find (i, j, k), that nums[i] + nums[j] + nums[k] = 0 (0 < i < j < k < N)
 * return non-duplicate [(nums[i], nums[j], nums[k]), ..]
 * convert to:
 * Find (j, k), that nums[j] +nums[k] = -nums[i] (j, k C- (i, N) )
 *
 * It's N times two-sum problem
 *
 * Complexity: O(N * logN + N * N) = O(N^2)
 */
#include "leet.h"
#include <stdio.h>
#include <stdlib.h>

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
                // append result
                size++;
                reallocM(&ret, sizeof(int *) * size);
                ret[size - 1] = p;
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
        // skip duplicate nums[i]
        while (nums[i] == nums[i + 1]) {
            i++;
        }
    }
    *returnSize = size;
    return ret;
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

int test(int *nums, int numsSize) {
    int size;
    int **ret = threeSum(nums, numsSize, &size);

    printf(">>> ");
    showArr(nums, numsSize);
    showArr2(ret, size, 3);
    int i, j;
    for (i = 0; i != size - 1; i++) {
        for (j = i + 1; j != size; j++) {
            if (cmpTri(ret[i], ret[j]) == 0) {
                printf("!!! find duplicate triplet\n");
                showArr(ret[i], 3);
                showArr(ret[j], 3);
            }
        }
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
