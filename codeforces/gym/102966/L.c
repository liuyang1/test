/**
 * @author      : lyz (lyz@$HOSTNAME)
 * @file        : L
 * @created     : Wednesday Jan 10, 2024 11:47:29 CST
 * https://codeforces.com/gym/102966/problem/L
 * AC   530ms 300KB
 *
 * https://blog.csdn.net/tongjingqi_/article/details/115579859
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int count_prime_factor(unsigned int x) {
    if (x <= 1) {
        return 0;
    }
    unsigned int i, r;
    for (i = 2, r = 0; i * i <= x; i++) {
        if (x % i == 0) {
            r++;
            do {
                x /= i;
            } while (x % i == 0);
        }
    }
    r += x != 1;
    return r;
}

unsigned int gcd(unsigned int a, unsigned int b) {
    if (b == 0) {
        assert(0);
    }
    unsigned int c;
    while (b != 0) {
        // a, b = b, a % b
        c = b;
        b = a % b;
        a = c;
    }
    return a;
}

int main() {
    // printf("%d\n", gcd(1071, 0));
    // printf("%d\n", count_prime_factor(1071));
    // printf("%d\n", count_prime_factor(17));
    // return 0;
    unsigned int n = 0;
    scanf("%u", &n);
    for (; n != 0; n--) {
        unsigned int a = 0, b = 0;
        scanf("%u%d", &a, &b);
        unsigned int r = count_prime_factor(a) +
            count_prime_factor(b) - count_prime_factor(gcd(a, b));
        printf("%u\n", r);
    }
    return 0;
}
