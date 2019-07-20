/** http://codeforces.com/problemset/problem/520/B */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int bound = 10000 * 10; // need check out of bound
    int sz = sizeof(int) * (bound + 1);
    int *a = malloc(sz);
    memset(a, 0x0, sz);
    int *b = malloc(sz);
    memset(b, 0x0, sz);
    int m, n;
    scanf("%u%u", &m, &n);

    int step = 1;
    a[m] = step;
    int top = 0, i;
    b[top++] = m;
    for (i = 0; i != top; i++) {
        int x = b[i];
        step = a[x] + 1;

        int v = x * 2;
        if (v == n) {
            break;
        }
        if (v <= bound && a[v] == 0) {
            a[v] = step;
            b[top++] = v;
        }
        v = x - 1;
        if (v == n) {
            break;
        }
        if (v <= bound && a[v] == 0) {
            a[v] = step;
            b[top++] = v;
        }
    }
    printf("%d\n", step - 1);
    free(a);
    free(b);
    return 0;
}
