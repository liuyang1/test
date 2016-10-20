#include "leet.h"
// stuipd solution, we could find better one.
int insert(int x, int *a, int idx, int max) {
    if (idx == 0) {
        a[0] = x;
        idx++;
    } else if (idx == 1) {
        if (x > a[0]) {
            a[1] = x;
            idx++;
        } else if (x < a[0]) {
            a[1] = a[0];
            a[0] = x;
            idx++;
        }
    } else if (idx == 2) {
        if (x < a[0]) {
            a[2] = a[1];
            a[1] = a[0];
            a[0] = x;
            idx++;
        } else if (x > a[0] && x < a[1]) {
            a[2] = a[1];
            a[1] = x;
            idx++;
        } else if (x > a[1]) {
            a[2] = x;
            idx++;
        }
    } else if (idx == 3) {
        if (x < a[0]) {
            // do nothing
        } else if (x > a[0] && x < a[1]) {
            a[0] = x;
        } else if (x > a[1] && x < a[2]) {
            a[0] = a[1];
            a[1] = x;
        } else if (x > a[2]) {
            a[0] = a[1];
            a[1] = a[2];
            a[2] = x;
        }
    }
    return idx;
}

int last(int *a, int idx, int max) {
    if (idx == max) {
        return a[0];
    } else {
        return a[idx - 1];
    }
}

int thirdMax(int *nums, int size) {
    // must not empty, not need check it.
    int m[3], idx = 0;
    int i;
    for (i = 0; i != size; i++) {
        idx = insert(nums[i], m, idx, 3);
    }
    return last(m, idx, 3);
}

bool unit(int *nums, int size, int e) {
    int r = thirdMax(nums, size);
    bool ret = r == e;
    if (!ret) {
        showArr(nums, size);
        printf("thirdMax(arr)=%d != %d %s\n", r, e, expect(ret));
    }
    return ret;

}

int main() {
    int A[] = {2, 2, 3, 1};
    unit(A, COUNT_OF(A), 1);
    int B[] = {3, 2, 1};
    unit(B, COUNT_OF(B), 1);
    int C[] = {1, 2};
    unit(C, COUNT_OF(C), 2);
    int D[] = {1, 2, 3, 4, 5, 6};
    unit(D, COUNT_OF(D), 4);
    return 0;
}
