#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int cmp(const void *a, const void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    return (*x) - (*y);
}

int dumb_closet(int *a, int n) {
    int i, j, s = a[0] + a[1];
    for (i = 0; i != n - 1; i++) {
        for (j = i + 1; j != n; j++) {
            int t = a[i] + a[j];
            if (abs(t) < abs(s)) {
                s = t;
            }
        }
    }
    return s;
}

void showArray(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%d, ", a[i]);
    }
    printf("\n");
}

/** solution
 * sum = a[i] + a[j], this is triangle
 * If a is sorted, the triangle is slope
 * Consider sum = abs(a[i]+a[j]), the triangle is simplex
 * We could search result by moving step by step from x-axis or y-axis
 */
int closet(int *a, int n) {
    qsort(a, n, sizeof(int), cmp);
    // showArray(a, n);
    int i, j, s, t;
    for (i = 0, j = n - 1, s = a[i] + a[j]; i < j; ) {
        t = a[i] + a[j];
        if (abs(t) < abs(s)) {
            s = t;
        }
        // printf("%d,%d %d,%d +=%d s=%d\n", i, a[i], j, a[j], t, s);
        if (t == 0) {
            break;
        } else if (t > 0) {
            j--;
        } else {
            i++;
        }
    }
    return s;
}

int *scanArray(int *out_n) {
    assert(out_n);
    int i, n;
    scanf("%d", &n);
    int *a = malloc(sizeof(int) * n);
    for (i = 0; i != n; i++) {
        scanf("%d", &a[i]);
    }
    *out_n = n;
    return a;
}

int main(int argc, char **argv) {
    int t;
    scanf("%d", &t);
    for (; t != 0; t--) {
        int n;
        int *a = scanArray(&n);
        assert(n >= 2);
        assert(a != NULL);
        int r = closet(a, n);
        printf("%d\n", r);
        // int r_ = dumb_closet(a, n);
        // if (r != r_) {
        //     printf("dumb way ans=%d\n", r_);
        // }
        free(a);
    }
    return 0;
}
