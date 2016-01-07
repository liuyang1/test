/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
    int *r = malloc(sizeof(int) * numsSize);
    int i, acc;
    for (i = 1, r[0] = 1; i != numsSize; i++) {
        r[i] = r[i - 1] * nums[i - 1];
    }
    acc = 1;
    for (i = numsSize - 2; i != -1; i--) {
        acc *= nums[i + 1];
        r[i] *= acc;
    }
    *returnSize = numsSize;
    return r;
}
