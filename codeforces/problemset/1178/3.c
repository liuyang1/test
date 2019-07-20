#include <stdio.h>

int k = 998244353;
int main() {
    int m, n;
    scanf("%d%d", &m, &n);
    int t = m + n, r = 1;
    while (t--) {
        r = (r * 2) % k;
    }
    printf("%d\n", r);
    return 0;
}
