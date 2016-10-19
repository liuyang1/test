#include "leet.h"
#include <stdio.h>
#include <stdlib.h>

// This function affect order in nums.
int threeSumClosest(int* nums, int numsSize, int target) {
    if (numsSize < 2 || nums == NULL) {
        assert(0);
    }
    qsort(nums, numsSize, sizeof(int), cmpInt);
    int ret = nums[0] + nums[1] + nums[2];
    unsigned int diff = abs(ret - target);
    int i;
    for (i = 0; i < numsSize - 2; i++) {
        int bgn, end;
        for (bgn = i + 1, end = numsSize - 1; bgn < end; ) {
            int sum = nums[i] + nums[bgn] + nums[end];
            unsigned int d = abs(sum - target);
            if (d < diff) {
                diff = d;
                ret = sum;
            }
            if (sum == target) {
                // find target, end loop
                goto end;
            } else if (sum > target) {
                end--;
            } else {
                bgn++;
            }
        }
    }
end:
    return ret;
}

// test code
int test(int *nums, int numsSize, int target, int e) {
    int ret = threeSumClosest(nums, numsSize, target);

    char s[128];
    strnArr(s, 128, nums, numsSize);
    printf(">>> threeSumClosest(%s, %d)\n", s, target);
    printf("%d\n", ret);

    bool r = ret == e;
    if (r) {
        printf("%s\n", expect(r));
    } else {
        printf("%s (expect %d)\n", expect(r), e);
    }
    return r;
}

int main() {
    int a[] = {-1, 0, 1, 2, -1, 4};
    test(a, sizeof(a) / sizeof(int), 0, 0);
    int b[]= {-1, 2, 1, -4};
    test(b, sizeof(b) / sizeof(int), 1, 2);
    return 0;
}
