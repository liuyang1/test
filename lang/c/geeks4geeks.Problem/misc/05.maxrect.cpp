#define MAX 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINX(a, b) ((a) < (b) ? (a) : (b))
#define MAXX(a, b) ((a) > (b) ? (a) : (b))

void show_vec(int *a, int n) {
    int i;
    for (i = 0; i != n; i++, a++) {
        printf("%d ", *a);
    }
    printf("\n");
}
void show_mat(int a[MAX][MAX], int n, int m, const char *label=NULL) {
    if (label == NULL) {
        label = "";
    }
    printf("%s a=%p %d,%d\n", label, a, n, m);
    int i;
    for (i = 0; i != n; i++) {
        show_vec(a[i], m);
    }
}
int combo(int *a, int n, int *t) {
    int i, c;
    for (i = c = 0; i != n; i++) {
        t[i] = c = (a[i] != 0) * (c + 1);;
    }
    show_vec(t, n);
}

int combine(int st0[MAX][MAX], int st1[MAX][MAX], int n, int m, int h) {
    int i, j;
    for (i = 0; i != n - 1; i++) {
        for (j = 0; j != m; j++) {
            int a = st0[i][j], b = st0[i + 1][j];
            st0[i][j] = MINX(a, b);
            int m = MAXX(st1[i][j], st1[i + 1][j]);
            st1[i][j] = MAXX(m, st0[i][j] * h);
        }
    }
}

int combo2(int a[MAX][MAX], int n, int m, int st0[MAX][MAX], int st1[MAX][MAX]) {
    memset(st0, 0, sizeof(int) * MAX * MAX);
    int i, j, c;
    for (i = 0; i != n; i++) {
        for (j = c = 0; j != m; j++) {
            st0[i][j] = c = (a[i][j] != 0) * (c + a[i][j] /* 1 */);
        }
    }
    // show_mat(a, n, m, "a");
    memcpy(st1, st0, sizeof(int) * MAX * MAX);
    int t;
    for (t = 0; t != n - 1; t++) {
        int h = n - t;
        combine(st0, st1, h, m, t + 2);
        // show_mat(st0, h - 1, m, "st0");
        // show_mat(st1, h - 1, m, "st1");
    }
    for (i = 1, j = st1[0][0]; i != m; i++) {
        j = MAXX(j, st1[0][i]);
    }
    return j;
}
int maxArea(int M[MAX][MAX], int n, int m) {
    static int st0[MAX][MAX], st1[MAX][MAX];
    return combo2(M, n, m, st0, st1);
}

int test_vec() {
    int a[10] = {1, 1, 1, 1, 0, 1, 0, 0, 1, 1};
    int b[10];
    combo(a, 10, b);
    show_vec(a, 10);
    show_vec(b, 10);
    return 0;
}
int test_comb2() {
    int a[MAX][MAX] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
    };
    int i;
    int st0[MAX][MAX], st1[MAX][MAX];
    combo2(a, 3, 10, st0, st1);
    // show_mat(a, 3, 10);
    // show_mat(st0, 3, 10);
    // show_mat(st1, 3, 10);
    int m = maxArea(a, 3, 10);
    printf("%d\n", m);
    return 0;
}

int main() {
    int t;
    for (scanf("%d", &t); t != 0; t--) {
        int n, m, i, j;
        scanf("%d%d", &n, &m);
        static int a[MAX][MAX];
        for (i = 0; i != n; i++) {
            for (j = 0; j != m; j++) {
                scanf("%d", &a[i][j]);
            }
        }
        int r = maxArea(a, n, m);
        printf("%d\n", r);
    }
    return 0;
}
