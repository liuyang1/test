int searchInsert(int* nums, int numsSize, int target) {
    int b, e, m;
    for (b = 0, e = numsSize - 1; b < e; ) {
        m = (b + e) / 2;
        if (nums[m] == target) {
            return m;
        } else if (target > nums[m]) {
            b = m + 1;
        } else {
            e = m - 1;
        }
    }
    m = (b + e) / 2;
    if (nums[m] >= target) { // insert lower position when equal
        return m;
    } else {
        return m + 1;
    }
}
