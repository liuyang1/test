#include <stdio.h>
#include <limits.h>

#include "leet.h"

int maxSubArray(int *nums, int numsSize) {
    long long s, max = LLONG_MIN, min = LLONG_MAX;
    int i;
    for (i = 0, s = 0; i != numsSize; i++) {
        s += nums[i];
        max = max < s ? s : max;
        min = min > s ? s : min;
    }
    printf("min=%lld max=%lld\n", min, max);
    if (max < 0) {
        return max;
    } else {
        return (int)(max - min);
    }
}

#define TEST(a, e) {                                      \
        int v = maxSubArray(a, sizeof(a) / sizeof(a[0])); \
        printf("%d ?= %d %s\n", v, e, expect(v == e));}

int main() {
    int a[] =  {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    TEST(a, 6);
    int b[] = {-1, 4, 1, -1};
    TEST(b, 5);
    int c[] = {-1, -2, -3};
    TEST(c, -1);
    int a4[] = {-1, 0, -2, -3};
    TEST(a4, 0);
    int a5[] = {-1, 1, -2, -3};
    TEST(a5, 1);

    return 0;
}
