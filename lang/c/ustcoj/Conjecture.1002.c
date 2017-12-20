#include <stdio.h>

int cycyleLen(int n) {
    if (n == 0) {
        return -1; // should assert this case
    }
    int cnt;
    for (cnt = 1; n != 1; cnt++) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }
    return cnt;
}

int maxCycleLen(int a, int b) {
    if (a > b) {
        return maxCycleLen(b, a);
    }
    int i, max = 0;
    for (i = a; i <= b; i++) {
        int n = cycyleLen(i);
        if (n > max) {
            max = n;
        }
    }
    return max;
}

int main() {
    int a, b;
    while (scanf("%d%d", &a, &b) == 2) {
        printf("%d %d %d\n", a, b, maxCycleLen(a, b));
    }
    return 0;
}
