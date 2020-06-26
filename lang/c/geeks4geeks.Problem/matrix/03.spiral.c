/** trasversing spirally */
/** 1: easy */

#include <stdio.h>
#include <stdlib.h>
void show_block(int *a, int w, int i0, int i1, int j0, int j1) {
    // printf("%d->%d %d-> %d\n", i0, i1, j0, j1);
    int i, j;
    int s0 = (i0 < i1) ? 1 : -1;
    int s1 = (j0 < j1) ? 1 : -1;
    for (i = i0; i != i1; i += s0) {
        for (j = j0; j != j1; j += s1) {
            printf("%d ", a[i * w + j]);
        }
    }
}
void show_spiral(int *a, int m, int n, int w) {
    show_block(a, w, 0, 1, 0, n);
    show_block(a, w, 1, m, n - 1, n);
    if (n == 1 || m == 1) {
        printf("\n");
        return;
    }
    show_block(a, w, m - 1, m, n - 2, -1);
    show_block(a, w, m - 2, 0, 0, 1);
    if (m == 2 || n == 2) {
        printf("\n");
        return;
    }
    show_spiral(a + w + 1, m - 2, n - 2, w);
}

int main() {
    int t, m, n, sz, *a, i;
    for (scanf("%d", &t); t != 0; t--) {
        for (scanf("%d%d", &m, &n), sz = m * n, a = malloc(sizeof(int) * sz), i = 0;
             i != sz; i++) {
            scanf("%d", &a[i]);
        }
        show_spiral(a, m, n, n);
        free(a);
    }
    return 0;
}
