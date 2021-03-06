#include "leet.h"
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(int *a, int n) {
    int i, t;
    for (i = 0; i != n / 2; i++) {
        t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}

void nextPermutation(int *nums, int numsSize) {
    int i, j;
    for (i = numsSize - 1; i > 0; i--) {
        if (nums[i] > nums[i - 1]) {
            break;
        }
    }
    for (j = numsSize - 1; j > 0; j--) {
        if (nums[j] > nums[i - 1]) {
            break;
        }
    }
    // For example:
    // 1    3    2
    // i-1  i    j
    // swap (i-1, j), and reverse [i, j]
    // 2    1    3
    if (i == 0) {
        reverse(nums + i, numsSize - i);
        return;
    }
    swap(nums + i - 1, nums + j);
    reverse(nums + i, numsSize - i);
}

void permEx(int *nums, int numsSize, int left, int right) {
    if (left == right) {
        showArr(nums, numsSize);
        return;
    }
    int i;
    for (i = left; i != right; i++) {
        swap(nums + left, nums + i);
        permEx(nums, numsSize, left + 1, right);
        swap(nums + left, nums + i);
    }
}

void perm(int *nums, int numsSize) {
    permEx(nums, numsSize, 0, numsSize);
}

void testPerm() {
    int a[] = {0, 1, 2, 3};
    perm(a, COUNT_OF(a));
}

#define CASE(a, e) { int *b = a;                      \
                     nextPermutation(b, COUNT_OF(a)); \
                     showArr(b, COUNT_OF(a));         \
                     printf("%s\n", expect(cmpArr(b, COUNT_OF(a), e, COUNT_OF(e)))); }
int main() {
    CASE(((int[]) {1, 2, 3}), ((int[]) {1, 3, 2}));
    CASE(((int[]) {1, 3, 2}), ((int[]) {2, 1, 3}));
    CASE(((int[]) {2, 1, 3}), ((int[]) {2, 3, 1}));
    CASE(((int[]) {2, 3, 1}), ((int[]) {3, 1, 2}));
    CASE(((int[]) {3, 1, 2}), ((int[]) {3, 2, 1}));
    CASE(((int[]) {3, 2, 1}), ((int[]) {1, 2, 3}));
    CASE(((int[]) {1, 1}), ((int[]) {1, 1}));
    CASE(((int[]) {}), ((int[]) {}));

    testPerm();
    return 0;
}
