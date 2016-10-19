#include "leet.h"

int *intersect(int *nums1, int nums1Size, int *nums2, int nums2Size, int *returnSize) {
    if (nums1Size == 0 || nums2Size == 0) {
        *returnSize = 0;
        return NULL;
    }
    qsort(nums1, nums1Size, sizeof(int), cmpInt);
    qsort(nums2, nums2Size, sizeof(int), cmpInt);
    showArr(nums1, nums1Size);
    showArr(nums2, nums2Size);

    int *ret = malloc(sizeof(int) * (nums1Size + nums2Size));
    int i, j, k;
    for (i = j = k = 0; i != nums1Size && j != nums2Size;) {
        if (nums1[i] == nums2[j]) {
            ret[k] = nums1[i];
            k++;
            i++;
            j++;
        } else if (nums1[i] < nums2[j]) {
            i++;
        } else if (nums1[i] > nums2[j]) {
            j++;
        }

    }
    *returnSize = k;
    return ret;
}

#define UNIT(nums1, nums2) {                                                        \
        int len;                                                                    \
        int *ret = intersect(nums1, COUNT_OF(nums1), nums2, COUNT_OF(nums2), &len); \
        showArr(ret, len); free(ret);                                               \
}

int main() {
    int n0[] = {4, 7, 9, 7, 6, 7};
    int n1[] = {5, 0, 0, 6, 1, 6, 2, 2, 4};
    UNIT(n0, n1);
    int m0[] = {1, 2, 2, 1};
    int m1[] = {2, 2};
    UNIT(m0, m1);
    int l0[] = {INT_MIN, 1, 2, 3};
    int l1[] = {1, INT_MIN, INT_MIN};
    UNIT(l0, l1);
    return 0;
}
