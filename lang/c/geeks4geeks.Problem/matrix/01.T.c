/** transpose square matrix */
/** 1: easy, in-palce soluiton
 *  0: beginner, simple solution
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b) {int _t = a; a = b; b = _t; }
#define IDX(i, j) m[(i) * n + (j)]

static inline void swap(int *m, int n, int i0, int j0, int i1, int j1) {
    // printf("%d,%d <-> %d,%d\n", i0, j0, i1, j1);
    SWAP(IDX(i0, j0), IDX(i1, j1))
}

void transpose(int *m, int n) {
    int i, j;
    for (i = 0; i != 2 * n - 1; i++) {
        for (j = MAX(0, i + 1 - n); j < i / 2 + i % 2; j++) {
            swap(m, n, j, i - j, i - j, j);
        }
    }
}

void show_mat(int *m, int n) {
    int i, j;
    for (i = 0; i != n; i++) {
        for (j = 0; j != n; j++) {
            printf("%d ", IDX(i, j));
        }
        printf("\n");
    }
}
void show_vec(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int test() {
    int m[] = {
        0, 1, 2, 3, 4,
        4, 0, 1, 2, 3,
        3, 4, 0, 1, 2,
        2, 3, 4, 0, 1,
        1, 2, 3, 4, 0
    };
    show_mat(m, 5);
    transpose(m, 5);
    show_mat(m, 5);
    return 0;
}

int main() {
    int T;
    int *m, n, i;
    for (scanf("%d", &T); T > 0; T--) {
        scanf("%d", &n);
        m = malloc(sizeof(int) * n * n);
        for (i = 0; i != n * n; i++) {
            scanf("%d", &m[i]);
        }
        transpose(m, n);
        show_vec(m, n * n);
        free(m);
    }
    return 0;
}
