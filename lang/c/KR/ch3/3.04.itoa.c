#include "../kr.h"

// signed integer is two-complement representation.
// so INT_MIN will overflow.

// itoa: convert n to characters in s
void itoa(int n, char s[]) {
    int i = 0, sgn = sign(n);
    // do-while to handle n == 0 case
    do {
        s[i++] = sgn * (n % 10) + '0';
    } while ((n /= 10) != 0);
    if (sgn < 0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

#define STRLEN 128
#define CASE(i) {char r[STRLEN], e[STRLEN];       \
                 itoa(i, r); sprintf(e, "%d", i); \
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
