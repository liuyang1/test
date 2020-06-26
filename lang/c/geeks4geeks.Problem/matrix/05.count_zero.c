/** counting zero in sorted matrix */
/** This matrix is so small, we could just use simple solution
 * 0: beginner
 *
 * binary search way, is 1: easy
 */

#define MAX 50
#include <stdio.h>
int countZeroes(int A[MAX][MAX], int n);
int main() {
    int t, n, i, j, A[MAX][MAX];
    for (scanf("%d", &t); t != 0; t--) {
        scanf("%d", &n);
        for (i = 0; i != n; i++) {
            for (j = 0; j != n; j++) {
                scanf("%d", &A[i][j]);
            }
        }
        printf("%d\n", countZeros(A, n));
    }
    return 0;
}
int countZeroes(int A[MAX][MAX], int n) {
    int i, j, c;
    for (i = c = 0; i != n; i++) {
        for (j = 0; j != n; j++) {
            c += A[i][j] == 0;
        }
    }
    return c;
}
