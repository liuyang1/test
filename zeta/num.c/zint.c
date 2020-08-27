#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
/** API design
 *
 * like register of computer, we could have different style
 * - zero operand, PUSH A, PUSH B, ADD, POP
 * - one operand, LOAD A, ADD B, STORE A
 * - two operands, ADD A B (A+=B)
 *   variable value, user need count memory management
 * - three operands, ADD C A B
 *   many intermediate value when do complex procedure
 * We pick two operands style, it save temp var handling issue;
 *
 * For memory, we use heap to save all data
 * We don't support zint_t use stack (or static memory), and we support API to
 * build zint_t from that memory
 * - ctor(int)
 * - ctor(char *s)
 *
 * For memory efficiency and implementation, we use uint32_t/uint8_t for internal
 * element
 *
 * Coding Style
 * - Leading with zint module name
 * - Prefer to passing value
 * - return value, so caller could chaing calling
 */

#include "zint.h"

typedef uint16_t delem_t; // it's double size than elem_t

#define LOG printf

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SET(x, sign_, s_, n_) {x.sign = sign_, x.s = s_, x.n = n_;}
zint_t ZINT_ZERO = {.sign = 0, .s = NULL, .n = 0};

/** internal function */
static inline elem_t idx(zint_t x, size_t i) {
    assert(0 <= i && i < x.n);
    return x.s[i];
}

/** add two positive integers */
static inline zint_t add_pos_c(zint_t a, zint_t b) {
    assert(a.sign == 1 && b.sign == 1);
    elem_t *s = malloc(sizeof(elem_t) * (MAX(a.n, b.n) + 1));
    size_t nn = MIN(a.n, b.n);
    size_t i;
    elem_t c = 0;
    zint_t r;
    for (i = 0; i != nn; i++) {
        c += idx(a, i) + idx(b, i), s[i] = c % BASE, c /= BASE;
    }
    for (; i != a.n; i++) {
        c += idx(a, i), s[i] = c % BASE, c /= BASE;
    }
    for (; i != b.n; i++) {
        c += idx(b, i), s[i] = c % BASE, c /= BASE;
    }
    if (c != 0) {
        s[i++] = c % BASE;
    }
    SET(r, 1, s, i);
    return r;
}

/** public function */
zint_t zint_copy(zint_t x, size_t n) {
    n = MAX(n, x.n);
    elem_t *s = malloc(sizeof(elem_t) * n);
    memcpy(s, x.s, x.n * sizeof(elem_t));
    zint_t r = {.sign = x.sign, .s = s, .n = x.n};
    return r;
}

bool zint_check(char *s) {
    if (*s == '+' || *s == '-') {
        s++;
    }
    for (; *s != '\0'; s++) {
        if (!isdigit(*s)) {
            return false;
        }
    }
    return true;
}

//                1, 10, 100,1000,10^4,10^5,10^6,10^7
elem_t tab_d[] = {1, 10, 100, 232, 16, 160, 64, 128};
elem_t tab_r[] = {0,  0,   0,   3, 39, 390, 3906, 39062};
zint_t zint_ctor_str(char *s) {
    assert(zint_check(s));
    int sign = 1;
    if (*s == '+') {
        sign = 1, s++;
    } else if (*s == '-') {
        sign = -1, s++;
    }
    size_t n = strlen(s);
    if (n == 0) {
        return ZINT_ZERO;
    }
    size_t m = (n / 2) + 1; // BASE > 10 * 10
    elem_t *seq = malloc(sizeof(elem_t) * m);
    const delem_t in_base = 10;
    delem_t c = 0;
    char *p;
    size_t i;
    for (p = s + n - 1, i = c = 0; p != s - 1; p--) {
        c = ((*p) - '0') + c * in_base;
        printf("c=%u\n", c);
        if (c >= BASE) {
            seq[i++] = c % BASE;
            c /= BASE;
            assert(i < m);
        }
    }
    if (c != 0) {
        seq[i++] = c % BASE;
    }
    zint_t r; SET(r, sign, seq, i);
    return r;
}

void zint_free(zint_t x) {
    if (x.s != NULL) {
        free(x.s);
    }
}

char sign_sym(int s) {
    if (s == 0) {
        return '0';
    } else if (s > 0) {
        return '+';
    } else {
        return '-';
    }
}

void zint_debug(zint_t x) {
    printf("sign=%c n=%zu\n", sign_sym(x.sign), x.n);
    size_t i;
    for (i = 0; i != x.n; i++) {
        printf("%03d,", idx(x, i));
    }
    printf("\n");
}

void zint_show(zint_t x) {
    if (x.sign < 0) {
        putchar('-');
    }
    size_t i;
    for (i = x.n - 1; i < x.n; i--) {
        printf("%d", idx(x, i));
    }
    if (x.n == 0) {
        printf("0");
    }
}

/** print x to string s
 * When s is NULL, return length??? */
size_t zint_print(char *s, size_t n, zint_t x);

/** public operator function */
int zint_sgn(zint_t a) {
    return a.sign;
}

zint_t zint_neg(zint_t x) {
    x.sign *= -1;
    return x;
}

int zint_cmp(zint_t a, zint_t b) {
    if (a.sign != b.sign) {
        return a.sign - b.sign;
    }
    if (a.n != b.n) {
        return a.n - b.n;
    }
    size_t i;
    for (i = a.n - 1; i < a.n; i--) {
        if (idx(a, i) != idx(b, i)) {
            return idx(a, i) - idx(b, i);
        }
    }
    return 0; // SAME
}

zint_t zint_add(zint_t a, zint_t b) {
    return a;
}

// for negative number, generate complete number, so we could unify add, minus operator
static inline zint_t complete(zint_t a, int n) {
    zint_t r = zint_copy(a, n);
    size_t i;
    for (i = 0; i != n; i++) {
        a.s[i] = BASE - a.s[i];
    }
    return r;
}

zint_t add(zint_t a, zint_t b) {
    return a;
}

zint_t zint_minus(zint_t a, zint_t b) {
    return b;
}

// shift to left, padding zero, NO discard
static inline zint_t Lshift_c(zint_t a, size_t n) {
    zint_t r = zint_copy(a, a.n + n);
    memset(r.s, 0, n), memcpy(r.s + n, a.s, a.n);
    r.n = a.n + n;
    return r;
}

static inline zint_t Rshift_c(zint_t a, size_t n) {
    zint_t r = zint_copy(a, 0);
    if (a.n > n) {
        r.n = a.n - n;
        memcpy(r.s, a.s + n, r.n);
    } else {
        r.n = 0;
    }
    return r;
}

// clear high/left part, n chars
static inline void clr_left(zint_t a, size_t n) {
    assert(n <= a.n);
    a.n -= n;
    memset(a.s + a.n, 0, n);
}

// x = a * 10^n + b
static inline void split(zint_t x, size_t n, zint_t *out_a, zint_t *out_b) {
    zint_t a = zint_copy(x, 0), b = zint_copy(x, 0);
    zint_t c = Rshift_c(a, n);
    clr_left(b, x.n - n);
    if (out_a) {
        *out_a = c;
    } else {
        zint_free(c);
    }
    if (out_b) {
        *out_b = b;
    } else {
        zint_free(b);
    }
    zint_free(a);
}

// Karatsuba algo
// (a*10^n + b)(c*10^n + d)
// = ac * 10^2n + (ad + bc) * 10^n + bd
// (ad+bc) = (a+b)(c+d) - ac - bd
// so we could use 3 multiplcation to recursion halv problem
zint_t zint_mul(zint_t x, zint_t y) {
    if (x.n == 0 || y.n == 0) {
        return ZINT_ZERO;
    }
    if (x.n == 1 || y.n == 1) {

    }
    zint_t a, b, c, d;
    size_t n = MIN(x.n, y.n);
    n /= 2;
    split(x, n, &a, &b), split(y, n, &c, &d);
    zint_t apb = add_pos_c(a, b); // a + b
    zint_t cpd = add_pos_c(c, d); // c + d
    zint_t m = zint_mul(apb, cpd); // (a + b) * (c + d)
    zint_t ac = zint_mul(a, c); // ac
    zint_t bd = zint_mul(b, d); // bd
    zint_t t0 = zint_minus(m, ac);
    zint_t t1 = zint_minus(t0, bd); // (a + b)(c + d) - ac - bd = bc + ad
    zint_t t2 = Lshift_c(t1, n);
    zint_t ac1 = Lshift_c(ac, n * 2);
    zint_t r0 = add_pos_c(bd, t2);
    zint_t r1 = add_pos_c(r0, ac1);
    return r1;
}
