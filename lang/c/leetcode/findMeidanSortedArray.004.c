#include <stdio.h>

double median2(int x, int y)
{
    return (x + y) / 2.0;
}

#define MIN(x, y)       ((x) < (y) ? (x) : (y))
#define MAX(x, y)       ((x) > (y) ? (x) : (y))

int findNthSortedArrays(int *nums1, int nums1Size,
                        int *nums2, int nums2Size,
                        int nth)
{
    int beg = 0, end = MIN(nums1Size - 1, nth);
    if (nums1Size == 0) {
        return nums2[nth];
    }
    if (nums2Size == 0) {
        return nums1[nth];
    }
    int mid;
    int val, high, low;
    int loop = 1;
    while (1) {
        mid = (beg + end) / 2;
        val = nums1[mid];
        low = nth - mid - 1 < 0 ? val: nums2[nth - mid - 1];
        high = nth - mid >= nums2Size ? low: nums2[nth - mid];
        // printf("xxx loop=%d nth=%d beg=%d end=%d mid=%d nums1=%p nums2=%p %d %d %d\n",
        //        loop++, nth, beg, end, mid, nums1, nums2, val, low, high);
        if (nth - mid - 1 >= nums2Size) {
            beg = (beg == mid) ? mid + 1 : mid;
        // } else if (nth - mid - 1 < 0) {
        } else if ((nth == 0 && val < low) ||
                   (nth == nums1Size + nums2Size - 1 && val > high) ||
                   (nth - mid >= nums2Size && val >= low) ||
                   (val >= low && val <= high)) {
            return val;
        } else if (val >= high) {
            end = mid;
        } else {
            beg = (beg == mid) ? mid + 1 : mid;
        }
        if ((beg == end && beg == mid) || beg >= nums1Size) {
            break;
        }
    }
    return findNthSortedArrays(nums2, nums2Size, nums1, nums1Size, nth);
}

double findMedianSortedArrays(int *nums1, int nums1Size, int *nums2, int nums2Size) {
    int size = nums1Size + nums2Size;
    if (size % 2 != 0) {
        return findNthSortedArrays(nums1, nums1Size, nums2, nums2Size,
                                   size / 2);
    } else {
        return median2(findNthSortedArrays(nums1, nums1Size, nums2, nums2Size,
                                           size / 2 - 1),
                       findNthSortedArrays(nums1, nums1Size, nums2, nums2Size,
                                           size / 2));
    }
}

void runCaseFind(int lineno, int *num1, int len1, int *num2, int len2, int nth, int expect)
{
    int m = findNthSortedArrays(num1, len1, num2, len2, nth);
    if (m != expect) {
        printf("case@%d ---- wrong nth(%d) %d expect %d\n", lineno, nth, m, expect);
    } else {
        printf("case@%d PASS\n", lineno);
    }
}
void runCase(int lineno, int *num1, int len1, int *num2, int len2, double median)
{
    double m = findMedianSortedArrays(num1, len1, num2, len2);
    if (m != median) {
        printf("case@%d wrong median %f expect %f\n", lineno, m, median);
    }
}

#define RUNFIND(a0, a1, nth, expect) \
    runCaseFind(__LINE__, a0, sizeof(a0) / sizeof(int), a1, sizeof(a1) / sizeof(int), nth, expect);

#define RUNCASE(a0, a1, median) \
    runCase(__LINE__, a0, sizeof(a0) / sizeof(int), a1, sizeof(a1) / sizeof(int), median);


void case0() {
    RUNFIND(((int[]) {2, 2, 4}), ((int[]) {1, 2, 3}), 0, 1);
    RUNFIND(((int[]) {2, 2, 4}), ((int[]) {1, 2, 3}), 1, 2);
    RUNFIND(((int[]) {2, 2, 4}), ((int[]) {1, 2, 3}), 2, 2);
    RUNFIND(((int[]) {2, 2, 4}), ((int[]) {1, 2, 3}), 3, 2);
    RUNFIND(((int[]) {2, 2, 4}), ((int[]) {1, 2, 3}), 4, 3);
    RUNFIND(((int[]) {2, 2, 4}), ((int[]) {1, 2, 3}), 5, 4);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {2, 2, 4}), 0, 1);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {2, 2, 4}), 1, 2);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {2, 2, 4}), 2, 2);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {2, 2, 4}), 3, 2);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {2, 2, 4}), 4, 3);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {2, 2, 4}), 5, 4);
}
void case1() {
    RUNFIND(((int[]) {0}), ((int[]) {1}),  0, 0);
    RUNFIND(((int[]) {0}), ((int[]) {1}),  1, 1);
    RUNFIND(((int[]) {1}), ((int[]) {0}), 0, 0);
    RUNFIND(((int[]) {1}), ((int[]) {0}), 1, 1);
}

void case2() {
    RUNFIND(((int[]) {0}), ((int[]) {1, 2, 3}),  0, 0);
    RUNFIND(((int[]) {0}), ((int[]) {1, 2, 3}),  1, 1);
    RUNFIND(((int[]) {0}), ((int[]) {1, 2, 3}),  2, 2);
    RUNFIND(((int[]) {0}), ((int[]) {1, 2, 3}),  3, 3);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {0}), 0, 0);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {0}), 1, 1);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {0}), 2, 2);
    RUNFIND(((int[]) {1, 2, 3}), ((int[]) {0}), 3, 3);
}
void case3() {
    RUNFIND(((int[]) {-1, 2, 3}), ((int[]) {0}), 0, -1);
    RUNFIND(((int[]) {-1, 2, 3}), ((int[]) {0}), 1, 0);
    RUNFIND(((int[]) {-1, 2, 3}), ((int[]) {0}), 2, 2);
    RUNFIND(((int[]) {-1, 2, 3}), ((int[]) {0}), 3, 3);
    RUNFIND(((int[]) {0}), ((int[]) {-1, 2, 3}),  0, -1);
    RUNFIND(((int[]) {0}), ((int[]) {-1, 2, 3}),  1, 0);
    RUNFIND(((int[]) {0}), ((int[]) {-1, 2, 3}),  2, 2);
    RUNFIND(((int[]) {0}), ((int[]) {-1, 2, 3}),  3, 3);
}
void case4() {
    RUNFIND(((int[]) {}), ((int[]) {1}), 0, 1);
    RUNFIND(((int[]) {}), ((int[]) {1}), 1, 1);
}


int main()
{
    case0();
    case1();
    case2();
    case3();
    case4();

    return 0;
}
