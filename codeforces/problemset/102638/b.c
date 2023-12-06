/**
 * raw solution get: Time limit exceeded on test 6
 */
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

/** a^n%m */
uint64_t powm(uint64_t a, uint64_t n, uint64_t m) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 1) {
        return (powm(a, n - 1, m) * a) % m;
    } else {
        uint64_t b = powm(a, n / 2, m) % m;
        return (b * b) % m;
    }
}

int main() {
    uint64_t n, m;
    while (scanf("%" PRIu64 "%" PRIu64, &n, &m) == 2) {
        uint64_t k = powm(n, m - 2, m);
        printf("%" PRIu64 "\n", k);
    }
    return 0;
}
