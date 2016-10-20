#include "leet.h"
int maxRotateFunction(int *A, int ASize) {
    if (A == NULL || ASize == 0) {
        return 0;
    }
    int sum, i, f, max;
    for (i = sum = f = 0; i != ASize; i++) {
        f += i * A[i];
        sum += A[i];
    }
    for (max = f, i = 1; i != ASize; i++) {
        // iterate result
        f = f + sum - ASize * A[ASize - i];
        if (f > max) {
            max = f;
        }
    }
    return max;
}

bool unit(int *a, int size, int e) {
    int r = maxRotateFunction(a, size);
    bool ret = r == e;
    if (!ret) {
        showArr(a, size);
        printf("maxRotateFunction(arr)=%d != %d %s\n",
               r, e, expect(ret));
    }
    return ret;
}

int main() {
    unit(NULL, 0, 0);
    int A[] = {4, 3, 2, 6};
    unit(A, COUNT_OF(A), 26);
    int B[] = {1, 2, 3, 4, 45, 5, 5, 12};
    unit(B, COUNT_OF(B), 394);
}
