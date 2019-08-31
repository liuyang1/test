#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool isPrime(uint64_t x) {
    uint64_t i;
    if (x % 2 == 0) {
        return false;
    }
    for (i = 3; i * i < x; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    uint64_t i;
    for (i = 2; i < 64; i++) {
        uint64_t x = (1ull << i) - 1;
        if (isPrime(x)) {
            printf("%llu %llu\n", i, x);
        }
    }
    return 0;
}
