#include "leet.h"

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *columnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int **subsets(int *nums, int numsSize, int **columnSizes, int *returnSize) {
    qsort(nums, numsSize, sizeof(int), cmpInt); // need non-descending order
    int sz = 1 << numsSize;
    *returnSize = sz;
    *columnSizes = malloc(sizeof(int) * sz);
    int **r = malloc(sizeof(int *) * sz);

    int i, j, k, c, v;
    for (i = 0; i != sz; i++) {
        v = i;
        // count 1-bit
        for (c = 0; v; c += 1) {
            v &= v - 1;
        }
        (*columnSizes)[i] = c;
        r[i] = malloc(sizeof(int) * c);
        for (j = 0, k = 0; j != numsSize; j++) {
            if (i & (1 << j)) {
                r[i][k] = nums[j];
                k++;
            }
        }
    }
    return r;
}

int main() {
    int nums[] = {2, 4, 3, 1, 5};
    int *colsz, retsz;
    int **r = subsets(nums, COUNT_OF(nums), &colsz, &retsz);
    int i;
    for (i = 0; i != retsz; i++) {
        printf("%3d: ", i);
        showArr(r[i], colsz[i]);
        free(r[i]);
    }
    free(colsz);
    free(r);
    return 0;
}
