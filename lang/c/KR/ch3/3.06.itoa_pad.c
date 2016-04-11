#include "../kr.h"

// signed integer is two-complement representation.
// so INT_MIN will overflow.

// itoa_pad: convert n to characters in s
// padded with blanks on the left if necessay to make it wide enough
void itoa_pad(int n, char s[], int width) {
    int i = 0, sgn = sign(n);
    // do-while to handle n == 0 case
    do {
        s[i++] = sgn * (n % 10) + '0';
    } while ((n /= 10) != 0);
    if (sgn < 0) {
        s[i++] = '-';
    }
    while (i < width) {
        s[i++] = ' ';
    }
    s[i] = '\0';
    reverse(s);
}

void itoa_sprintf(int n, char s[], int width) {
#define FMTLEN 16
    char fmt[FMTLEN];
    snprintf(fmt, FMTLEN, "%%%dd", width);
    sprintf(s, fmt, n);
}

#define STRLEN 128
#define CASE(n, w) {char r[STRLEN], e[STRLEN];       \
                 itoa_pad(n, r, w); itoa_sprintf(n, e, w); \
                 printf("%d [%s] ?= [%s] %s\n",       \
                        n, r, e, expect(strcmp(r, e) == 0)); }

int main() {
    CASE(0, 4);
    CASE(123, 4);
    CASE(-123, 5);
    CASE(1234, 5);
    CASE(-1234, 5);
    CASE(INT_MAX, 10);
    CASE(INT_MIN, 16);
    return 0;
}

