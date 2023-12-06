#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
 
/** a^n%m */
uint64_t powm(uint64_t a, uint64_t n, uint64_t m) {
    if (n == 0) {
        return 1;
    }
    if (n % 2 == 1) {
        uint64_t t0 = powm(a, n - 1, m);
        uint64_t r = t0 * a;
        printf("%"PRIu64 " %"PRIu64" =%"PRIu64"\n", t0, a, r);
        if (t0 >= 2 && a >= 2) {
                assert(r > t0 && r > a);
        }
        return (powm(a, n - 1, m) * a) % m;
    } else {
        uint64_t b = powm(a, n / 2, m);
        uint64_t r = b * b;
        printf("%"PRIu64 " ** =%"PRIu64"\n", b, r);
        if (b >= 2) {
                assert(r > b);
        }
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
