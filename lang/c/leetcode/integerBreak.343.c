#include "leet.h"
/**
 * dynamic programming with recursion
 */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
int integerBreakIn(int *tbl, int n) {
    if (n == 1) {
        tbl[1] = 1;
    } else if (tbl[n] == 0) {
        int i, m, t, a, b;
        for (i = 1, m = 0; i <= n - 1; i++) {
            a = integerBreakIn(tbl, i);
            a = MAX(a, i);
            b = integerBreakIn(tbl, n - i);
            b = MAX(b, n - i);
            t = a * b;
            if (t > m) {
                m = t;
            }
        }
        tbl[n] = m;
    }
    return tbl[n];
}

int integerBreak(int n) {
    int *tbl = malloc(sizeof(int) * (n + 1));
    memset(tbl, 0, sizeof(int) * (n + 1));
    int r = integerBreakIn(tbl, n);
    free(tbl);
    return r;
}

// test code

bool unit(int n, int e) {
    int ret = integerBreak(n);
    bool r = ret == e;
    if (!r) {
        printf("integerBreak(%d) = %d != %d %s\n",
               n, ret, e, expect(r));
    }
    return r;
}

int main() {
    unit(2, 1);
    unit(10, 36);
    unit(4, 4);
    unit(11, 54);
    unit(12, 81);
    unit(15, 243);
    unit(20, 1458);
    unit(31, 78732);
    return 0;
}
