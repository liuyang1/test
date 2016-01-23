#include "leet.h"
void rotate(int** matrix, int matrixRowSize, int matrixColSize) {
    int i, j, t, z = matrixRowSize;
    if (matrixRowSize != matrixColSize) {
        return;
    }
    for (i = 0; i != z / 2; i++) {
        // printf("n %d t: %d %d\n", z, i, z - 2 * i - 1);
        for (j = i; j != z - i - 1; j++) {
            t = matrix[i][j];
            matrix[i][j] = matrix[z - 1 - j][i];
            matrix[z - 1 - j][i] = matrix[z - 1 - i][z - 1 - j];
            matrix[z - 1 - i][z - 1 - j]  = matrix[j][z - 1 - i];
            matrix[j][z - 1 - i] = t;
        }
    }
}

int **buildArr2(int n) {
    int i, j, v = 0;
    int **m = malloc(sizeof(int *) * n);
    for (i = 0; i != n; i++) {
        int *mm = malloc(sizeof(int) * n);
        m[i] = mm;
        for (j = 0; j != n; j++) {
            mm[j] = v++;
        }
    }
    return m;
}

int test(int n) {
    int **m = buildArr2(n);
    // showArr2(m, n, n);
    rotate(m, n, n);
    showArr2(m, n, n);
    int i;
    for (i = 0; i != n; i++) {
        free(m[i]);
    }
    free(m);
}

int main() {
    test(1);
    test(2);
    test(3);
    test(4);
    test(5);
    test(6);
}
