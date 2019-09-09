#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

bool isPrime(uint64_t x) {
    if (x == 2) {
        return true;
    }
    uint64_t i;
    for (i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

int gen_twin_prime() {
    uint64_t i;
    bool last_prime_2 = false;
    for (i = 3; i <= 10000; i += 2) {
        if (isPrime(i)) {
            if (last_prime_2) {
                printf("%"PRIu64" %"PRIu64"\n", i - 2, i);
            }
            last_prime_2 = true;
        } else {
            last_prime_2 = false;
        }
    }
    return 0;
}

char *num2digit(uint64_t x, size_t *plen) {
    char *r = malloc(sizeof(char) * 100);
    size_t i;
    for (i = 0; x != 0; i++) {
        r[i] = x % 10;
        x /= 10;
    }
    *plen = i;
    return r;
}

uint64_t digit2num(char *d, size_t len) {
    uint64_t x = 0;
    size_t i;
    for (i = len; ; i--) {
        x = 10 * x + d[i];
        if (i == 0) {
            break;
        }
    }
    return x;
}

void reverseDigit(char *d, size_t len) {
    char c;
    size_t i;
    for (i = 0; i != len / 2; i++) {
        char *p0 = d + i;
        char *p1 = d + len - 1 - i;
        c = *p0;
        *p0 = *p1;
        *p1 = c;
    }

}

void showDigit(char *d, size_t len) {
    size_t i;
    for (i = 0; i != len; i++) {
        printf("%d,", d[i]);
    }
    printf("\n");
}

int test_rev() {
    size_t len;
    char *d = num2digit(1234567890, &len);
    showDigit(d, len);
    reverseDigit(d, len);
    showDigit(d, len);
    uint64_t x = digit2num(d, len);
    printf("%"PRIu64"\n", x);
    free(d);
    return 0;
}

uint64_t revdigit(uint64_t x) {
    size_t len;
    char *d = num2digit(x, &len);
    reverseDigit(d, len);
    uint64_t y = digit2num(d, len);
    free(d);
    return y;
}

int gen_rev_prime() {
    uint64_t x;
    for (x = 11; x < 10000; x += 2) {
        if (isPrime(x)) {
            uint64_t y = revdigit(x);
            if (x < y && isPrime(y)) {
                printf("%"PRIu64" %"PRIu64"\n", x, y);
            }
        }
    }
    return 0;
}

int gen_pandlidrom_prime() {
    uint64_t x;
    for (x = 11; x < 1000000; x += 2) {
        if (isPrime(x)) {
            uint64_t y = revdigit(x);
            if (x == y) {
                printf("%"PRIu64"\n", x);
            }
        }
    }
    return 0;
}

int main() {
    // gen_rev_prime();
    gen_pandlidrom_prime();
    return 0;
}
