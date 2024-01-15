/**
 * @author      : lyz (lyz@$HOSTNAME)
 * @file        : L
 * @created     : Wednesday Jan 10, 2024 11:47:29 CST
 * https://codeforces.com/gym/102966/problem/L
 * AC   530ms 300KB
 *
 * https://blog.csdn.net/tongjingqi_/article/details/115579859
 *
 * Sieve FAIL runtime error on test case 9
 * I already add sanity check, but still failing.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define N   4000 // N*N=10004569>10**7
static unsigned int p[N];
static unsigned int cnt_prime;
void init_sieve() {
    memset(p, 1, sizeof(p));
    p[0] = p[1] = 0;
    unsigned int i, j;
    for (i = 2; i != N ; i++) {
        if (p[i]) {
            for (j = i * 2; j < N; j += i) {
                p[j] = 0;
            }
        }
    }
    // p[prime_index] = 1 => p=[prime]
    for (i = j = 0; j != N; j++) {
        if (p[j]) {
            p[i++] = j;
        }
    }
    cnt_prime = i;
}

void show_sieve() {
    unsigned int i;
    for (i = 0; i != cnt_prime; i++) {
        printf("%u, ", p[i]);
    }
    printf("\n");
}


unsigned int count_prime_factor_sieve(unsigned int x) {
    if (x <= 1) {
        return 0;
    }
    // printf("x=%u\n", x);
    unsigned int i, r;
    for (i = 0, r = 0; p[i] * p[i] <= x; i++) {
        // printf("check prime p[i=%u]=%u\n", i, p[i]);
        if (x % p[i] == 0) {
            r++;
            // printf("find prime p[i=%u]=%u\n", i, p[i]);
            do {
                x /= p[i];
            } while (x % p[i] == 0);
        }
    }
    r += x != 1;
    // printf("r=%u res=%u\n", r, x);
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

int rand_test() {
    unsigned int i, r = 0, t;
    for (i = 0; i != 100 * 1000; i++) {
        unsigned int a = rand() % (10 * 1000 * 1000) + 1;
        unsigned int b = rand() % (10 * 1000 * 1000) + 1;
        r += t = count_prime_factor(a) +
            count_prime_factor(b) - count_prime_factor(gcd(a, b));
        if (i < 10) {
            printf("a=%u b=%u t=%u\n", a, b, t);
        }
    }
    printf("%u\n", r);
    return 0;
}

unsigned int (*fn)(unsigned int x) = NULL;

int std_test() {
    unsigned int n = 0;
    scanf("%u", &n);
    for (; n != 0; n--) {
        unsigned int a = 0, b = 0;
        scanf("%u%d", &a, &b);
        unsigned int r = fn(a) + fn(b) - fn(gcd(a, b));
        printf("%u\n", r);
    }
    return 0;
}

int sanity_test() {
    unsigned int a, b;
    for (a = 1; a != 10 * 1000 * 1000 + 1; a++) {
        unsigned int r = count_prime_factor(a);
        unsigned int e = count_prime_factor_sieve(a);
        if (r != e) {
            printf("a=%u r=%u e=%u\n", a, r, e);
            return -1;
        }
    }
    return 0;
}

int sanity_test_2dim() {
    unsigned int a, b;
    for (a = 1; a != 10 * 1000 * 1000 + 1; a++) {
        for (b = 1; b != 10 * 1000 * 1000 + 1; b++) {
            unsigned int r = count_prime_factor(a) + count_prime_factor(b) - count_prime_factor(gcd(a, b));
            unsigned int e = count_prime_factor_sieve(a) + count_prime_factor_sieve(b) - count_prime_factor_sieve(gcd(a, b));
            if (r != e) {
                printf("a=%u b=%u r=%u e=%u\n", a, b, r, e);
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    // printf("%d\n", gcd(1071, 0));
    // printf("%d\n", count_prime_factor(1071));
    // printf("%d\n", count_prime_factor(17));
    // return 0;
    init_sieve();
    // show_sieve();
    // return 0;
    // sanity_test();
    // sanity_test_2dim();
    // rand_test();
    // fn = count_prime_factor;
    fn = count_prime_factor_sieve;
    std_test();
    return 0;
}
