#include "../kr.h"

static char gBaseTbl[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static int gBaseMax = sizeof(gBaseTbl) / sizeof(char);
char itobmap(int base, int v) {
    if (base > gBaseMax || v >= base) {
        return '?';
    }
    return gBaseTbl[v];
}

int itob(int n, char *s, int lim, int base) {
    int i = 0, sgn = sign(n);
    do {
        s[i++] = itobmap(base, sgn * (n % base));
    } while ((n /= base) != 0);
    if (sgn < 0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
    return i;
}

int btoimap(int base, char v) {
    if (v >= '0' && v <= '9') {
        return v - '0';
    } else if (v >= 'a' && v <= 'z') {
        return v - 'a' + 10;
    }
    return v;
}

int btoi(char *s, int base) {
    int r = 0, sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    for (; *s != '\0'; s++) {
        r *= base;
        r += sign * btoimap(base, *s);
    }
    return r;
}

int buildFmt(char *fmt, int n, int base) {
    int i = 0;
    if (n < 0) {
        fmt[i++] = '-';
    }
    char c;
    switch (base) {
        case 8: c = 'o'; break;
        case 16: c = 'x'; break;
        case 10:
        default: c = 'u'; break;
    }
    fmt[i++] = '%';
    fmt[i++] = c;
    fmt[i] = '\0';
    return i;
}

int itob_sprintf(int n, char *s, int lim, int base) {
    char fmt[4];
    buildFmt(fmt, n, base);
    sprintf(s, fmt, abs(n));
    return 0;
}

#define STRLEN 128
#define CASE(n, base) {char r[STRLEN], e[STRLEN];                  \
                       itob(n, r, STRLEN, base);                   \
                       itob_sprintf(n, e, STRLEN, base);           \
                       int rr = btoi(r, base);                     \
                       printf("%d %s ?= %s %s\n",                  \
                              n, r, e, expect(strcmp(r, e) == 0)); \
                       printf("rev %d ?= %d %s\n",                 \
                              n, rr, expect(n == rr)); }

void test_base(int base) {
    CASE(0, base);
    CASE(123, base);
    CASE(-123, base);
    CASE(1234, base);
    CASE(-1234, base);
    CASE(INT_MAX, base);
    CASE(INT_MIN, base);
}

int main() {
    test_base(8);
    test_base(10);
    test_base(16);
    return 0;
}
