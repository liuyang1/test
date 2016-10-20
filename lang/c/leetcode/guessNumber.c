#include "leet.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int getMoneyAmount(int n) {
    int **tab = malloc(sizeof(*tab) * (n + 1));
    int i;
    for (i = 0; i != n + 1; i++) {
        tab[i] = malloc(sizeof(*tab[i]) * (n + 1));
        memset(tab[i], 0, sizeof(tab[i][0]) * (n + 1));
    }

    int b, e, k, m, v;
    for (e = 1; e != n + 1; e++) {
        for (b = e - 1; b != 0; b--) {
            m = INT_MAX;
            for (k = b; k != e; k++) {
                // How to find worst case?
                // select k, and we have two possible cases, left or right,
                //      assume worst case.
                v = k + MAX(tab[b][k - 1], tab[k + 1][e]);
                if (m > v) {
                    m = v;
                }
            }
            tab[b][e] = m;
        }
    }
    showArr2(tab, n + 1, n + 1);

    int r = tab[1][n];
    for (i = 0; i != n + 1; i++) {
        free(tab[i]);
    }
    free(tab);
    return r;
}

bool unit(int n, int e) {
    int r = getMoneyAmount(n);
    bool ret = r == e;
    if (!ret) {
        printf("getMoneyAmount(%d) = %d != %d %s\n",
               n, r, e, expect(ret));
    }
    return ret;
}

int main() {
    unit(1, 0);
    unit(2, 1);
    unit(3, 2);
    unit(4, 4);
    unit(5, 6);
    unit(6, 8);
    unit(7, 10);
    unit(8, 12);
    unit(9, 14);
    unit(10, 16);
    unit(100, 400);
    return 0;
}
