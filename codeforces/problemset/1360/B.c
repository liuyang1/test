/** codeforces.com/problemset/problem/1360/B */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *x, const void *y) {
    const int *a = x, *b = y;
    return *a - *b;
}

int main() {
#define LEN     50
    int t, n, *s = malloc(sizeof(int) * LEN), i, m;
    for (scanf("%d", &t); t != 0; t--) {
        scanf("%d", &n);
        assert(n >= 2);
        for (i = 0; i != n; i++) {
            scanf("%d", s + i);
        }
        qsort(s, n, sizeof(int), cmp);
        for (i = 1; i != n; i++) {
            s[i - 1] = abs(s[i] - s[i - 1]);
        } // [ti] -> [ti+1-ti]
        for (i = 1, m = s[0]; i != n - 1; i++) {
            if (s[i] < m) {
                m = s[i];
            }
        }
        printf("%d\n", m);
    }
    free(s);
    return 0;
}
