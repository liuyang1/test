#include "../kr.h"

char *itoa_r_i(int n, char s[]) {
    if (n == 0) {
        return s;
    }
    s = itoa_r_i(n / 10, s);
    *s = -1 * (n % 10) + '0'; // n is negative num
    return s + 1;
}
// itoa_r: convert n to characters in s with recursion style
void itoa_r(int n, char s[]) {
    int sgn = sign(n);
    if (sgn == -1) {
        *s++ = '-';
    } else {
        // convert negative num to postive num will overflow for INT_MIN
        // so do it on the contray
        n = -n;
    }
    if (n == 0) {
        *s++ = '0';
    } else {
        s = itoa_r_i(n, s);
    }
    *s = '\0';
}

#define STRLEN 128
#define CASE(i) {char r[STRLEN], e[STRLEN];       \
                 itoa_r(i, r); sprintf(e, "%d", i); \
                 printf("%d %s ?= %s %s\n",       \
                        i, r, e, expect(strcmp(r, e) == 0)); }

int main() {
    CASE(0);
    CASE(123);
    CASE(-123);
    CASE(1234);
    CASE(-1234);
    CASE(INT_MAX);
    CASE(INT_MIN);
    return 0;
}
