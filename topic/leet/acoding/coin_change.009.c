#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int coin_change_iter(int *denom, int n, int k, int amount) {
    if (amount <= 0 || k < 0) {
        return 0;
    }
    printf("amount=%d n[%d]=%d\n", amount, k, denom[k]);
    if (k == 0 && amount % denom[k] == 0) {
        return 1;
    }
    int t;
    int s = 0;
    for (; k >= 0; k--) {
        t = amount;
        for (; t > 0;) {
            t -= denom[k];
            s += coin_change_iter(denom, n, k - 1, t);
        }
    }
    return s;
}

int coin_change(int *denom, int n, int amount) {
    return coin_change_iter(denom, n, n - 1, amount);
}

int main() {
    int a[] = {1, 2, 5};
    int r = coin_change(a, sizeof(a) / sizeof(a[0]), 7);
    printf("%d\n", r);
    return 0;
}
