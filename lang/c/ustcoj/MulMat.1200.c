#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A(p*q) * B(r*s) == C(n*m)
bool isMul(int **A, int p, int q, int **B, int r, int s, int ** C, int n, int m) {
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

int **readMat(int *pp, int *pq) {
    int p, q, i, j;
    scanf("%d%d", &p, &q);
    if (p == 0 && q == 0) {
        return NULL;
    }
    int **M = malloc(sizeof(int *) * p);
    for (i = 0; i != p; i++) {
        M[i] = malloc(sizeof(int) * q);
        for (j = 0; j != q; j++) {
            scanf("%d", &M[i][j]);
        }
    }
    *pp = p;
    *pq = q;
    return M;
}
void freeMat(int **M, int p, int q) {
    int i;
    for (i = 0; i != p; i++) {
        free(M[i]);
    }
    free(M);
}
int main() {
    while (true) {
        int p, q, r, s, n, m;
        int **A = readMat(&p, &q);
        if (A == NULL) {
            break;
        }
        int **B = readMat(&r, &s), **C = readMat(&n, &m);
        printf("%s\n", isMul(A, p, q, B, r, s, C, n, m) ? "Yes" : "No");
        freeMat(A, p, q);
        freeMat(B, r, s);
        freeMat(C, n, m);
    }
    return 0;
}
