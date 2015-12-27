int removeDuplicates(int* nums, int numsSize) {
    int i, j;
    if (numsSize <= 2) {
        return numsSize;
    }
    for (i = 2, j = 2; i != numsSize; i++) {
        if (nums[i] == nums[j - 1] && nums[i] == nums[j - 2]) {
        } else {
            if (j != i) {
                nums[j] = nums[i];
            }
            j++;
        }
    }
    return j;
}
