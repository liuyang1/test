#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
uint64_t comb(uint64_t a, uint64_t b) {
    if (a - b < b) {
        return comb(a, a - b);
    }
    if (b == 0) {
        return 1;
    }
    uint64_t r = 1;
    size_t i;
    for (i = 1; i <= b; i++) {
        r = r * (a + 1 - i) / i;
    }
    return r;
}

uint64_t comb1(uint64_t a, uint64_t b) {
    if (a - b > b) {
        return comb(a, a - b);
    }
    uint64_t *p = malloc(sizeof(uint64_t) * b);
    size_t i;
    for (i = 0; i != b; i++) {
        p[i] = 1;
    }
    for (i = 1; i <= b; i++) {
        p[i] += p[i-1];
    }
    return p[b-1];
}

int main() {
    uint64_t x;
    for (x = 0; x != 10; x++) {
        uint64_t i;
        for (i = 0; i != x + 1; i++) {
            printf("%"PRIu64", ", comb1(x, i));
        }
        printf("\n");
    }
    return 0;
}
