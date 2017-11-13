#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

char *decompose(char *nrStr, char *drStr) {
    unsigned long long a = strtoull(nrStr, NULL, 10);
    unsigned long long b = strtoull(drStr, NULL, 10);
    size_t len = 4096;
    char *r = malloc(sizeof(char) * len);
    size_t offset = 0;
    if (a > b) {
        offset += snprintf(r + offset, len - offset, "%llu,", a / b);
        a = a - b * (a / b);
    }
    while (a != 0) {
        unsigned long long den = ceil(b / (a + 0.0));
        a = a * den - b;
        b = b * den;
        offset += snprintf(r + offset, len - offset, "1/%llu,", den);
    }
    if (offset > 1) {
        offset--; // remove last comma
    }
    r[offset] = '\0';
    return r;
}

bool unit_decompose(char *nr, char *dr, char *expect) {
    char *ret = decompose(nr, dr);
    bool r = strcmp(ret, expect) == 0;
    // if (!r) {
    printf("decompose(%s, %s) = %s ?= %s\n", nr, dr, ret, expect);
    // }
    free(ret);
    return r;
}

int test_decompose() {
    unit_decompose("3", "4", "1/2,1/4");
    unit_decompose("12", "4", "3");
    unit_decompose("4", "5", "1/2,1/4,1/20");
    unit_decompose("66", "100", "1/2,1/7,1/59,1/5163,1/53307975");
    unit_decompose("22", "23", "1/2,1/3,1/9,1/83,1/34362");
    return 0;
}

int main() {
    test_decompose();
    return 0;
}
