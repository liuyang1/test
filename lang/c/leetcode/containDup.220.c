bool containsNearbyAlmostDuplicate(int* nums, int numsSize, int k, int t) {
    int i, j;
    if (numsSize == 0) {
        return false;
    }
    for (i = 0; i != numsSize - 1; i++) {
        for (j = i + 1; j != i + k + 1 && j != numsSize; j++) {
            if (llabs(((long long)nums[i]) - ((long long)nums[j])) <= t) {
                return true;
            }
        }
    }
    return false;
}
