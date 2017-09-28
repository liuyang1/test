#include "leet.h"

#define MAX(a, b)   ((a) > (b) ? (a) : (b))
int lineRob(int *nums, int numsSize) {
    if (numsSize <= 0) {
        return 0;
    } else if (numsSize == 1) {
        return nums[0];
    }
    int prev1 = nums[0], prev0 = MAX(nums[0], nums[1]), v;
    size_t i;
    for (i = 2; i != numsSize; i++) {
        v = MAX(prev0, prev1 + nums[i]);
        prev1 = prev0;
        prev0 = v;
    }
    return prev0;
}

int rob(int *nums, int numsSize) {
    if (numsSize <= 0) {
        return 0;
    } else if (numsSize == 1) {
        return nums[0];
    }
    return MAX(lineRob(nums, numsSize - 1),
               lineRob(nums + 1, numsSize - 1));
}

#define CASE(a, e)     {int r = rob(a, sizeof(a) / sizeof(int)); \
                        printf("%d ?= %d %s\n", r, e, expect(r == e)); }
int main() {
    CASE(((int[]) {}), 0);
    CASE(((int[]) {3}), 3);
    CASE(((int[]) {4, 5, 9, 2, 2, 10}), 19);
    CASE(((int[]) {4, 5, 9}), 9);
    CASE(((int[]) {2, 1, 1, 2}), 3);
    return 0;
}
