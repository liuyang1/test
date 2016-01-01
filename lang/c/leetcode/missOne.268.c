int missingNumber(int* nums, int numsSize) {
    int i;
    unsigned long long sum;
    for (i = 0, sum = 0; i != numsSize; i++) {
        sum += nums[i];
    }
    return numsSize * (numsSize + 1) / 2 - sum;
}
