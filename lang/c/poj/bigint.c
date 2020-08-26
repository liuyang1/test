#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
/** simple big data with char string
 * support add, multiply, minus, divide, modulo
 */

/** API design
 *
 * like register of computer, we could have different style
 * - zero operand, PUSH A, PUSH B, ADD, POP
 * - one operand, LOAD A, ADD B, STORE A
 * - two operands, ADD A B (A+=B)
 * - three operands, ADD C A B
 * We pick two operands style, it save temp var handling issue;
 *
 * For memory, we use heap to save all data
 * We don't support INT use stack (or static memory), and we support API to
 * build INT from that memory
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

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef uint8_t elem_t;
#define BASE    256

typedef struct {
    int sign;
    size_t n; // length of sequence
    elem_t *s; // sequence, should not leading with zero
} zint_t;
/** For 0, sign=0, n = 0, s = NULL
 * For positive number, sign=1, n=N, s=
 * For negative number, sign=-1, n=N, s=
 */

/** internal function */
static inline elemt_t idx(zint_t x, size_t i) {
    assert(0 <= i && i < x.n);
    return x.s[i];
}

/** public function */
static inline zint_t zint_copy(zint_t x, size_t n) {
    n = MAX(n, x.n);
    elem_t *s = malloc(sizeof(elem_t) * n);
    memcpy(s, x.s, x.n * sizeof(elem_t));
    zint_t r = {.sign = x.sign, .s = s, .n = x.n};
    return r;
}

static inline void zint_free(zint_t x) {
    free(x.s);
}

static inline void zint_show(INT x) {
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
static inline size_t zint_print(char *s, size_t n, zint_t x);

/** public operator function */
static inline int zint_sgn(INT a) {
    return a.sign;
}

static inline zint_t zint_neg(zint_t x) {
    if (x.n == 0)
    x.

    INT r = copy_c(a, 0);
    r.sign = -1 * sgn(a);
    return r;
}

static inline int cmp(INT a, INT b) {
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
    return 0;
}

/** add two positive integers */
static inline INT add_pos_c(INT a, INT b) {
    assert(a.sign == 1 && b.sign == 1);
    char *s = malloc(sizeof(char) * (MAX(a.n, b.n) + 1));
    size_t nn = MIN(a.n, b.n);
    size_t i;
    char c = 0;
    INT r;
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
    r.s = s, r.n = i, r.sign = 1, r.heap = true;
    return r;
}

// for negative number, generate complete number, so we could unify add, minus operator
static inline INT complete(INT a, int n) {
    INT r = copy_c(a, n);
    size_t i;
    for (i = 0; i != n; i++) {
        a.s[i] = BASE - a.s[i];
    }
    return r;
}

static inline INT add(INT a, INT b) {
    return a;
}

static inline INT minus_c(INT a, INT b) {
    return b;
}

// shift to left, padding zero, NO discard
static inline INT Lshift_c(INT a, size_t n) {
    INT r = copy_c(a, a.n + n);
    memset(r.s, 0, n), memcpy(r.s + n, a.s, a.n);
    r.n = a.n + n;
    return r;
}
static inline INT Rshift_c(INT a, size_t n) {
    INT r = copy_c(a, 0);
    if (a.n > n) {
        r.n = a.n - n;
        memcpy(r.s, a.s + n, r.n);
    } else {
        r.n = 0;
    }
    return r;
}
// clear high/left part, n chars
static inline void clr_left(INT a, size_t n) {
    assert(n <= a.n);
    a.n -= n;
    memset(a.s + a.n, 0, n);
}
// x = a * 10^n + b
static inline void split(INT x, size_t n, INT *out_a, INT *out_b) {
    INT a = copy_c(x, 0), b = copy_c(x, 0);
    INT c = Rshift_c(a, n);
    clr_left(b, x.n - n);
    if (out_a) {
        *out_a = c;
    } else {
        free_int(c);
    }
    if (out_b) {
        *out_b = b;
    } else {
        free_int(b);
    }
    free_int(a);
}

// Karatsuba algo
// (a*10^n + b)(c*10^n + d)
// = ac * 10^2n + (ad + bc) * 10^n + bd
// (ad+bc) = (a+b)(c+d) - ac - bd
// so we could use 3 multiplcation to recursion halv problem
static inline INT mul_c(INT x, INT y) {
    if (x.n == 0 || y.n == 0) {
        return zero;
    }
    if (x.n == 1 || y.n == 1) {

    }
    INT a, b, c, d;
    size_t n = MIN(x.n, y.n);
    n /= 2;
    split(x, n, &a, &b), split(y, n, &c, &d);
    INT apb = add_pos_c(a, b); // a + b
    INT cpd = add_pos_c(c, d); // c + d
    INT m = mul_c(apb, cpd); // (a + b) * (c + d)
    INT ac = mul_c(a, c); // ac
    INT bd = mul_c(b, d); // bd
    INT t0 = minus_c(m, ac);
    INT t1 = minus_c(t0, bd); // (a + b)(c + d) - ac - bd = bc + ad
    INT t2 = Lshift_c(t1, n);
    INT ac1 = Lshift_c(ac, n * 2);
    INT r0 = add_pos_c(bd, t2);
    INT r1 = add_pos_c(r0, ac1);
    return r1;
}

int test_add_pos() {
    char s0[] = {1, 9, 1};
    char s1[] = {9, 4, 8, 9};
    INT a = {.sign = 1, .s = s0, .n = sizeof(s0) / sizeof(char), .heap = false};
    INT b = {.sign = 1, .s = s1, .n = sizeof(s1) / sizeof(char), .heap = false};
    INT c = add_pos_c(a, b);
    show(a), putchar('+'), show(b), putchar('='), show(c), printf("\n");
    free_int(a), free_int(b), free_int(c);
    return 0;
}
int test_shift() {
    char s0[] = {1, 9, 1};
    INT a = {.sign = 1, .s = s0, .n = sizeof(s0) / sizeof(char), .heap = false};
    INT b = Lshift_c(a, 4);
    show(b), putchar('\n');
    INT c = Rshift_c(b, 2);
    show(c), putchar('\n');
    free_int(a), free_int(b), free_int(c);
    return 0;
}
int test_mul() {
    char s0[] = {1, 9, 1};
    char s1[] = {9, 4, 8, 9};
    INT a = {.sign = 1, .s = s0, .n = sizeof(s0) / sizeof(char), .heap = false};
    INT b = {.sign = 1, .s = s1, .n = sizeof(s1) / sizeof(char), .heap = false};
    INT c = mul_c(a, b);
    show(a), putchar('*'), show(b), putchar('='), show(c), printf("\n");
    free_int(a), free_int(b), free_int(c);
    return 0;
}

int main() {
    // test_add_pos();
    test_mul();
    return 0;
}
