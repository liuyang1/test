#include "leet.h"
#include <stdlib.h>

#define MIN(a, b)       ((a) < (b) ? (a) : (b))
int minimumTotal(int **triangle, int triangleRowSize, int *triangleColSizes) {
    int sz = triangleColSizes[triangleRowSize - 1];
    int *r = malloc(sizeof(int) * sz);
    int i, j;
    for (i = 0; i != triangleColSizes[0]; i++) {
        r[i] = triangle[0][i];
    }
    for (; i != sz; i++) {
        r[i] = 0;
    }
    for (j = 1; j != triangleRowSize; j++) {
        sz = triangleColSizes[j];
        r[sz - 1] = r[sz - 2] + triangle[j][sz - 1];
        for (i = triangleColSizes[j] - 2; i > 0; i--) {
            r[i] = MIN(r[i], r[i - 1]) + triangle[j][i];
        }
        r[0] = r[0] + triangle[j][0];
    }
    int min = r[0];
    for (i = 1; i != triangleColSizes[triangleRowSize - 1]; i++) {
        if (r[i] < min) {
            min = r[i];
        }
    }
    free(r);
    return min;
}

int case0() {
    int a0[] = {2};
    int a1[] = {3, 4};
    int a2[] = {6, 5, 7};
    int a3[] = {4, 1, 8, 3};
    int *aa[] = {a0, a1, a2, a3};
    int col[] = {1, 2, 3, 4};
    int r = minimumTotal(aa, sizeof(col) / sizeof(int), col);
    int e = 11;
    printf("%d ?= %d %s\n", r, e, expect(r == e));
    return 0;
}

int main() {
    case0();
    return 0;
}
