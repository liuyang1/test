/**
 * Note: The returned array must be malloced, assume caller calls free().
 * Complexity: O(numSize ^ 2)
 */

int* twoSum(int* nums, int numsSize, int target) {
    int *ret = (int *)malloc(sizeof(int) * 2);
    int i, j;
    int sum;
    for (i = 0; i != numsSize; i++) {
        for (j = i + 1; j != numsSize; j++) {
            sum = nums[i] + nums[j];
            if (sum == target) {
                ret[0] = i + 1;
                ret[1] = j + 1;
                return ret;
            }
        }
    }
    // should not here
    free(ret);
    return NULL;
}
