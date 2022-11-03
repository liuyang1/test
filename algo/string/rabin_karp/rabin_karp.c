#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// largest prime, less than 2*24
#define Q4CHAR           (16777199)
#define value_t         int32_t

static inline
value_t add_mod(value_t a, value_t b, value_t m) {
    return (a + b) % m;
}

static inline
value_t minus_mod(value_t a, value_t b, value_t m) {
    return (a - b) % m;
}

static inline
value_t mul_mod(value_t a, value_t b, value_t m) {
    value_t ab = a * b;
    if (a > 1 && b > 1 && (ab < a || ab < b)) {
        printf("a=%d b=%d => ab=%d\n", a, b, ab);
        assert(ab >= a);
        assert(ab >= b);
    } // other assert
    return ab % m;
}

static inline
value_t pow_mod(value_t a, value_t n, value_t m) {
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return a;
    }
    if (n % 2 == 0) {
        value_t x = pow_mod(a, n / 2, m);
        return mul_mod(x, x, m);
    } else {
        return mul_mod(a, pow_mod(a, n - 1, m), m);
    }
}

// a + b * c
static inline
value_t mac_mod(value_t a, value_t b, value_t c, value_t m) {
    return add_mod(a, mul_mod(b, c, m), m);
}

static inline
bool str_eq(char *T, char *P, size_t m) {
    size_t i;
    for (i = 0; i != m; i++) {
        if (T[i] != P[i]) {
            return false;
        }
    }
    return true;
}

size_t rabin_karp_matcher(char *T, char *P, size_t n, size_t m, value_t d, value_t q)
{
    assert(n >= m);
    value_t h = pow_mod(d, m - 1, q);
    // printf("h=%d n=%zu m=%zu\n", h, n, m);
    value_t p = 0, t0 = 0;
    size_t i;
    for (i = 0; i != m; i++) {
        p = mac_mod(P[i], d, p, q);
        t0 = mac_mod(T[i], d, t0, q);
    }
    size_t cnt = 0;
    for (i = 0; i != n - m; i++) {
        if (p == t0) {
            if (str_eq(P, T + i, m)) {
                cnt++;
                // printf("patterns find with shift i=%zu\n", i);
            } else {
                // printf("suprious hit i=%zu\n", i);
            }
        }
        // printf("(%d - %d * %d) * %d + %d mod %d\n", t0, T[i], h, d, T[m + i], q);
#if 0
        t0 = ((t0 - T[i] * h) * d + T[m + i]) % q;
        t0 = (t0 + q) % q;
#else
        t0 = (t0 - T[i] * h) * d + T[m + i];
        t0 = t0 % q;
        t0 = t0 + (t0 < 0) * q;
#endif
    }
    return cnt;
}

char *str2digits(char *s) {
    char *p;
    for (p = s; *p != '\0'; p++) {
        // printf("s[i]=%p, %d\n", p, *p);
        *p = *p - '0';
    }
    return s;
}

size_t naive_matcher(char *T, char *P, size_t n, size_t m) {
    size_t i, cnt = 0;
    for (i = 0; i != n - m; i++) {
        if (str_eq(T + i, P, m)) {
            cnt++;
            // printf("patterns find with shift i=%zu\n", i);
        }
    }
    return cnt;
}

int exer_32_2_1() {
    char *T = "3141592653589793";
    char *P = "26";
    T = str2digits(strdup(T));
    P = str2digits(strdup(P));
    value_t q = 11;
    rabin_karp_matcher(T, P, strlen(T), strlen(P), 10, q);
    // naive_matcher(T, P, strlen(T), strlen(P));

    free(T);
    free(P);
    return 0;
}

int exer_32_2_1_char() {
    char *T = "3141592653589793";
    char *P = "2653";
    rabin_karp_matcher(T, P, strlen(T), strlen(P), 256, 65521);
    return 0;
}

/** AGUC, test string 1M, pattern 1024 */

/** digit, 0-9, test string 1M, pattern 1024 */

/** alphabet, a-z, test string 1M, pattern 1024 */

/** base64, a-z,A-z,0-9,+=, test string 1M, pattern 1024 */

/** char, test string 1M, pattern 1024 */
// 4, 10, 26, 64, 256
char *rand_str(size_t len, size_t mod) {
    char *p = malloc(sizeof(char) * (len + 1));
    size_t i;
    for (i = 0; i != len; i++) {
        p[i] = random() % mod;
    }
    return p;
}

size_t bench(size_t mod, size_t test_len, size_t pattern_len, value_t q) {
    srandom(42);
    char *t = rand_str(test_len, mod);
    char *p = rand_str(pattern_len, mod);

#if 1
    size_t cnt = rabin_karp_matcher(t, p, test_len, pattern_len, mod, q); // 1.85s
#else
    size_t cnt = naive_matcher(t, p, test_len, pattern_len); // 1.48s
#endif

    free(p);
    free(t);
    return cnt;
}

int exer() {
    exer_32_2_1();
    exer_32_2_1_char();
    return 0;
}

int main() {
    size_t cnt = bench(2, 64 * 1024 * 1024, 700, 32719);
    printf("cnt=%zu\n", cnt);
    return 0;
}
