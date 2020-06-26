/** convert a[i][j] <= 1, if a[i][j'] == 1 || a[i'][j] == 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
bool any(int *a, int w, int i0, int i1, int j0, int j1) {
    int i, j;
    for (i = i0; i != i1; i++) {
        for (j = j0; j != j1; j++) {
            if (a[i * w + j]) {
                return true;
            }
        }
    }
    return false;
}

int show_mat(int *a, int m, int n) {
    int i, j;
    for (i = 0; i != m; i++) {
        for (j = 0; j != n; j++) {
            printf("%d ", a[i * n + j]);
        }
        printf("\n");
    }
}

void boolean(int *a, int m, int n) {
    bool *r = malloc(sizeof(bool) * m);
    bool *c = malloc(sizeof(bool) * n);
    int i, j;
    for (i = 0; i != m; i++) {
        r[i] = any(a, n, i, i + 1, 0, n);
    }
    for (i = 0; i != n; i++) {
        c[i] = any(a, n, 0, m, i, i + 1);
    }
    for (i = 0; i != m; i++) {
        for (j = 0; j != n; j++) {
            a[i * n + j] = r[i] || c[j];
        }
    }
    free(r);
    free(c);
}

int main() {
    int t, m, n, *a, i, mn;
    for (scanf("%d", &t); t != 0; t--) {
        for (scanf("%d%d", &m, &n), mn = m * n, a = malloc(sizeof(int) * mn), i = 0;
             i != mn; i++) {
            scanf("%d", &a[i]);
        }
        boolean(a, m, n);
        show_mat(a, m, n);
        free(a);
    }
    return 0;
}
