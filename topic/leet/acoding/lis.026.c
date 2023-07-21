/** longest increasing subsequence of an array
 * findNumberOfLIS
 * DP
 */

#include <stdio.h>
#include <stdlib.h>

void show(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%d, ", a[i]);
    }
    printf("\n");
}

/** solution begin */
#define MAX(a, b) ((a) > (b))? (a):(b)
int findNumberOfLIS(int *a, int n) {
    int *l = malloc(sizeof(int) * n); // length
    int *f = malloc(sizeof(int) * n); // frequency

    int i, j, lm, fm, t;
    l[0] = 1;
    for (i = 0; i != n; i++) {
        f[i] = 1;
    }
    for (i = 1;  i != n; i++) {
        for (j = 0, lm = fm = 1; j != i; j++) {
            if (a[i] > a[j]) {
                t = l[j] + 1;
                if (t > lm) {
                    fm = f[j];
                    lm = t;
                } else if (t == lm) {
                    fm += f[j];
                }
            }
        }
        l[i] = lm;
        f[i] = fm;
    }
    // show(a, n); show(l, n); show(f, n);
    for (i = lm = fm = 0; i != n; i++) {
        if (l[i] > lm) {
            lm = l[i];
            fm = f[i];
        } else if (l[i] == lm) {
            fm += f[i];
        }
    }
    free(l);
    free(f);
    return fm;
}

/** solution end */

int unit(int *a, int n, int e) {
    n = findNumberOfLIS(a, n);
    printf("%d\n", n);
    if (n != e) {
        printf("fail e=%d\n", e);
    }
    return 0;
}

int main() {
    int a[] = {50, 3, 10, 7, 48, 80};
    unit(a, sizeof(a) / sizeof(a[0]), 2); // 3,{10,7},48,80, len=4,freq=1
    int b[] = {1, 2, 3, 4, 5, 6, 7, 100, 98, 99};
    unit(b, sizeof(b) / sizeof(b[0]), 1); // 1,2,3,4,5,6,7,98,99, len=9,freq=1
    int c[] = {3, 10, 2, 1, 20};
    unit(c, sizeof(c) / sizeof(c[0]), 1); // 3,10,20, len=3, freq=1
    int d[] = {3, 10, 2, 1, 4, 5, 20};
    unit(d, sizeof(d) / sizeof(d[0]), 3); // {3,2,1},4,5,20, len=4, freq=3
    {
        int a[] = {2, 2, 2, 2};
        unit(a, sizeof(a) / sizeof(a[0]), 4); // 2, len=1, freq=4
    }
    {
        int a[] = {1, 3, 5, 4, 7};
        unit(a, sizeof(a) / sizeof(a[0]), 2); // 1,3,{5,4},7, len=3, freq=2
    }

    return 0;
}
