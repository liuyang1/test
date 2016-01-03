#include "leet.h"

void merge(int* nums1, int m, int* nums2, int n) {
    int i;
    // move all data in nums1 from n till to end
    for (i = m - 1; i >= 0; i--) {
        nums1[n + i] = nums1[i];
    }
    int i1 = n, i2 = 0, j;
    i = 0;
    while (1) {
        if (i1 == m + n) {
            for (j = i2; j != n; j++, i++) {
                nums1[i] = nums2[j];
            }
            break;
        } else if (i2 == n) {
            break;
        }
        // Just work like merge with extra m + n size
        if (nums1[i1] <= nums2[i2]) {
            nums1[i] = nums1[i1];
            i++;
            i1++;
        } else {
            nums1[i] = nums2[i2];
            i++;
            i2++;
        }
    }
}

int isIncrease(int *nums, int len) {
    int i;
    for (i = 1; i != len; i++) {
        if (nums[i] <= nums[i - 1]) {
            return false;
        }
    }
    return true;
}

int test(int *nums1, int m, int *nums2, int n) {
    int *p = malloc(sizeof(int) * (m + n + 1));
    p[m + n] = NIL;
    int i;
    for (i = 0; i != m; i++) {
        p[i] = nums1[i];
    }
    memset(p + m, 0, sizeof(int) * n);
    showArr(nums1, m);
    showArr(nums2, n);
    merge(p, m, nums2, n);
    showArr(p, m + n + 1);
    int r = isIncrease(p, m + n);
    free(p);
    return r;
}
#define CASE(a, b) { \
    printf("%s\n", \
           test(a, sizeof(a) / sizeof(int), \
                b, sizeof(b) / sizeof(int)) ? "PASS" : "fail"); }
int main() {
    CASE(((int []){1}), ((int []){}));
    CASE(((int []){}), ((int []){1, 2}));
    CASE(((int []){1, 3, 5}), ((int []){}));
    CASE(((int []){2, 3, 6}), ((int []){1, 4, 5}));
    CASE(((int []){1, 4, 5}), ((int []){2, 3, 6}));
    CASE(((int []){1, 3, 5, 6, 10}), ((int []){2, 4, 7, 8}));
    CASE(((int []){2, 4, 7, 8}), ((int []){1, 3, 5, 6, 10}));
    CASE(((int []){1, 9, 99}), ((int []){0, 4, 6, 8, 10, 20}));

    return 0;
}
