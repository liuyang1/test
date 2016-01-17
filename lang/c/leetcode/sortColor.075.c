#include "leet.h"
typedef enum {
    red = 0,
    white,
    blue,
    max,
} Color;
void sortColors(int* nums, int numsSize) {
    int cnt[max] = {0};
    int i, j;
    for (i = 0; i != numsSize; i++) {
        cnt[nums[i]]++;
    }
    for (i = 1; i != max; i++) {
        cnt[i] += cnt[i - 1];
    }
    LOG("%s", "acc arr: "); showArr(cnt, max);
    for (i = 0, j = 0; i != numsSize; i++) {
        while (i == cnt[j]) {
            j = j + 1;
        }
        if (i < cnt[j]) {
            nums[i] = j;
        }
    }
}

void test(int *nums, int numsSize) {
    sortColors(nums, numsSize);
    showArr(nums, numsSize);
}
#define CASE(a) {test(a, sizeof(a) / sizeof(int)); }
int main() {
    CASE(((int []) {0}));
    CASE(((int []) {0, 2}));
    CASE(((int []) {2, 0}));
    CASE(((int []) {0, 1, 2}));
    CASE(((int []) {0, 1, 2, 1, 0, 2}));
    CASE(((int []) {0, 2, 2, 1, 0, 2}));
    return 0;
}
