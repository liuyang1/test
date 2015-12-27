int removeDuplicates(int* nums, int numsSize) {
    int i, j;
    if (numsSize <= 1) {
        return numsSize;
    }
    for (i = 1, j = 1; i != numsSize; i++) {
        if (nums[i] == nums[j - 1]) {
        } else {
            if (j != i) {
                nums[j] = nums[i];
            }
            j++;
        }
    }
    return j;
}
