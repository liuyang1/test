/** https://codeforces.com/gym/103488/problem/H */

/** 结果R(n)=2**n-1 */

#include <stdio.h>
#include <stdint.h>

/** quick power of 2 */
int64_t pow2(int64_t n) {
    if (n == 0) {
        return 1;
    } else if (n % 2 == 0) {
        int64_t t = pow2(n / 2);
        return (t * t) % 998244353;
    } else {
        return (pow2(n - 1) * 2) % 998244353;
    }
}

int main() {
    int t;
    scanf("%d", &t);
    for (; t != 0; t--) {
        int n, r = 0;
        scanf("%d", &n);
#if 0
        r = 0;
        for (; n != 0; n--) {
            r = (2 * r + 1) % 998244353;
        }
#else
        r = pow2(n);
        r -= 1;
#endif
        printf("%d\n", r);
    }
    return 0;
}
