int combination(int m, int n) {
    if (m - n < n) {
        n = m - n;
    }
    int i, j;
    unsigned long long p0 = 1;
    // i from m to 1, j from 1 to n, this keep dividable, and p0 is not
    // overflow in middle-progress
    for (i = m, j = 1; j != n + 1; i--, j++) {
        p0 = (i * p0) / j;
    }
    return p0;
}
int uniquePaths(int m, int n) {
    return combination(m + n - 2, m - 1);
}

#include <stdio.h>
#define CASE(m, n , e) {int x = uniquePaths(m, n); \
    printf("%d %d %d ?= %d %s\n", \
           m, n, x, e, x == e ? "PASS" : "fail"); }

int main() {
    CASE(10, 15, 817190);
    CASE(15, 10, 817190);
    return 0;
}
