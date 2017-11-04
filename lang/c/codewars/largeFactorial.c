#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INCLUDED
#include "multiplyString.c"
#undef INCLUDED

// for large factorial self
#define DIGITLEN    4096
char *factorial(int n) {
    if (n < 0) {
        return strdup("");
    }
    if (n == 0) {
        return strdup("1");
    }
    char *r = strdup("1"), *a, *b;
    b = malloc(sizeof(char) * DIGITLEN);
    for (int i = 1; i != n + 1; i++) {
        snprintf(b, DIGITLEN, "%d", i);
        a = r;
        r = multiply(a, b);
        free(a);
    }
    free(b);
    return r;
}

bool unit(int n, const char *s) {
    char *ret = factorial(n);
    bool r = strcmp(ret, s) == 0;
    printf("factorial(%d) = %s ?= %s\n", n, ret, s);
    free(ret);
    return r;
}

int test() {
    unit(-1, "");
    unit(0, "1");
    unit(5, "120");
    unit(25, "15511210043330985984000000");
    return 0;
}

int main() {
    test();
    return 0;
}
