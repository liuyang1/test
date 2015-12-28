int singleNumber(int* nums, int numsSize) {
    int ret, i;
    for (i = 0, ret = 0; i != numsSize; i++) {
        ret ^= nums[i];
    }
    return ret;
}
