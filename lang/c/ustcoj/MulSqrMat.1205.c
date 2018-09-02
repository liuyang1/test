#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A(p*q) * B(r*s) == C(n*m)
bool isMul(int **A, int p, int q, int **B, int r, int s,
           int **C, int n, int m) {
    if (!(q == r && p == n && s == m)) {
        return false;
    }
    int i, j, k;
    for (i = 0; i != n; i++) {
        for (j = 0; j != m; j++) {
            int sum = 0;
            for (k = 0; k != q; k++) {
                sum += A[i][k] * B[k][j];
            }
            if (C[i][j] != sum) {
                return false;
            }
        }
    }
    return true;
}

int **readMat(int n) {
    int i, j;
    int **M = malloc(sizeof(int *) * n);
    for (i = 0; i != n; i++) {
        M[i] = malloc(sizeof(int) * n);
        for (j = 0; j != n; j++) {
            scanf("%d", &M[i][j]);
        }
    }
    return M;
}

void freeMat(int **M, int n) {
    int i;
    for (i = 0; i != n; i++) {
        free(M[i]);
    }
    free(M);
}

int main() {
    while (true) {
        int n;
        scanf("%d", &n);
        if (n == 0) {
            break;
        }
        int **A = readMat(n), **B = readMat(n), **C = readMat(n);
        printf("%s\n", isMul(A, n, n, B, n, n, C, n, n) ? "Yes" : "No");
        freeMat(A, n);
        freeMat(B, n);
        freeMat(C, n);
    }
    return 0;
}
