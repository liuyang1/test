/**
 * convert from rational number to contigous fraction number
 */
#include <stdint.h>
#include <math.h>
#include <stdio.h>

typedef struct rational {
    uint32_t num;
    uint32_t den;
} rational;

uint32_t rat2contf(rational x, uint32_t *a, size_t len) {
    rational y;
    a[0] = x.num / x.den;
    y.den = (x.num - a[0] * x.den);
    if (y.den == 0) {
        return 1;
    }
    y.num = x.den;
    uint32_t l = rat2contf(y, a + 1, len - 1);
    return l + 1;
}

uint32_t rat2contf_loop(rational x, uint32_t *a, size_t len) {
    uint32_t t;
    uint32_t i;
    for (i = 0; i != len; i++) {
        a[i] = x.num / x.den;
        t = x.den;
        x.den = x.num - a[i] * x.den;
        x.num = t;
        if (x.den == 0) {
            return i + 1;
        }
    }
}

void showRat(rational x) {
    printf("%d/%d\n", x.num, x.den);
}

void showContF(uint32_t *a, size_t len) {
    uint32_t i;
    printf("[");
    for (i = 0; i != len; i++) {
        printf("%d", a[i]);
        if (i == 0) {
            printf(";");
        } else if (i != len - 1) {
            printf(",");
        }
    }
    printf("] len=%lu\n", len);
}

int unit(rational x) {
    showRat(x);
    uint32_t a[20] = {0};
    uint32_t len = rat2contf(x, a, 20);
    // uint32_t len = rat2contf_loop(x, a, 20);
    showContF(a, len);
    return 0;
}

int main() {
    rational x = {.num = 147, .den = 160};
    unit(x);

    x.num = 415;
    x.den = 93;
    unit(x);

    x.num = 31415927;
    x.den = 10000000;
    unit(x);

    x.den = 1000 * 1000 * 1000;
    x.num = sqrt(2) * x.den;
    unit(x);
    return 0;
}
