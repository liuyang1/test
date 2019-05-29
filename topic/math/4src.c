/**
 * 44100/48000 = 147/160
 * find one fraction q/p, minizie abs(q/p-147/160)
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Rat {
    uint32_t num;
    uint32_t den;
} Rat;

bool isPrime(uint32_t x) {
    if (x == 2) {
        return true;
    }
    if (x % 2 == 0) {
        return false;
    }
    uint32_t i;
    for (i = 3; i * i < x; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

void showRat(Rat x) {
    printf("%u/%u\n", x.num, x.den);
}

uint32_t gcd(uint32_t m, uint32_t n) {
    uint32_t t;
    do {
        t = m % n;
        m = n;
        n = t;
    } while (t != 0);
    return m;
}

int farey(Rat x, Rat y) {
    uint32_t k;
    Rat z;
    for (k = 1; k != 10; k++) {
        z.num = x.num + k * y.num;
        z.den = x.den + k * y.den;
        uint32_t t = gcd(z.num, z.den);
        z.num /= t;
        z.den /= t;
        if (!isPrime(z.num) && !isPrime(z.den)) {
            showRat(z);
        }
    }
    return 0;
}

int main() {
    Rat x = {.num = 1, .den = 1};
    Rat y = {.num = 11, .den = 12};
    farey(x, y);
    // Rat x = {.num = 11, .den = 12};
    // Rat y = {.num = 34, .den = 37};
    // farey(x, y);
    return 0;
}
