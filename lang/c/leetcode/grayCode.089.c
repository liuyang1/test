#include "leet.h"

int *grayCode(int n, int *returnSize) {
    if (n < 0) {
        *returnSize = 0;
        return NULL;
    }
    if (n == 0) {
        *returnSize = 1;
        int *r = malloc(sizeof(int) * (*returnSize));
        r[0] = 0;
        return r;
    }
    int sz1;
    int *r1 = grayCode(n - 1, &sz1);
    *returnSize = 2 * sz1;
    int *r = malloc(sizeof(int) * (*returnSize));
    memcpy(r, r1, sizeof(int) * sz1);

    int i, bias = 1 << (n - 1);
    for (i = sz1; i != *returnSize; i++) {
        // reflect and add bias, That's why Gray code also named
        //      Reflected-Binary-Code
        r[i] = r[*returnSize - 1 - i] + bias;
    }
    free(r1);
    return r;
}

int main() {
    int rank = 5;
    int sz;
    int *r = grayCode(rank, &sz);
    int i, j;
    for (i = 0; i != sz; i++) {
        // show binary code
        for (j = rank - 1; j >= 0; j--) {
            printf("%d", r[i] & (1 << j) ? 1 : 0);
        }
        printf("\n");
    }
    return 0;
}
