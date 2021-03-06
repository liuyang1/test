bool containsNearbyDuplicate(int *nums, int numsSize, int k) {
    int i, j;
    if (numsSize == 0) {
        return false;
    }
    for (i = 0; i != numsSize - 1; i++) {
        for (j = i + 1; j != i + k + 1 && j != numsSize; j++) {
            if (nums[i] == nums[j]) {
                return true;
            }
        }
    }
    return false;
}
