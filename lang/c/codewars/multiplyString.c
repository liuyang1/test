#include <sys/time.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int char2digit(char c) {
    return c - '0';
}

char digit2char(int d) {
    return d + '0';
}

size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

char *skipZeros(char *s) {
    while (*s == '0') {
        s++;
    }
    return s;
}

char *checkZero(char *s) {
    if (strcmp(s, "") == 0) {
        free(s); // all string is allocated, must free it.
        s = strdup("0");
    }
    return s;
}

// return pointer to chaining function call
char *reverse(char *p) {
    size_t len = strlen(p), i;
    for (i = 0; i != len / 2; i++) {
        size_t r = len - i - 1;
        char t = p[i];
        p[i] = p[r];
        p[r] = t;
    }
    return p;
}

char *add_scaler_in(char *a, char *b, int scaler) {
    size_t la = strlen(a), lb = strlen(b);
    size_t m = max(la, lb);
    char *c = malloc(sizeof(char) * (m + 2));
    size_t i;
    int carry;
    for (i = 0, carry = 0; i != m; i++) {
        int v, v0, v1;
        v0 = i < la ? char2digit(a[i]) : 0;
        v1 = i < lb ? char2digit(b[i]) : 0;
        v = v0 + scaler * v1 + carry;
        carry = (v >= 10) - (v < 0);
        v -= 10 * carry;
        c[i] = digit2char(v % 10);
    }
    if (carry > 0) {
        c[i++] = digit2char(carry);
    } else if (carry == 0) {
    } else {
        printf("'%s' '%s' %d %d %d => carry=%d ", a, b, la, lb, scaler, carry);
        printf("unexpected carray is negative\n");
        assert(0);
    }
    c[i] = '\0';
    return c;
}

char *add_in(char *a, char *b) {
    return add_scaler_in(a, b, 1);
}
char *sub_in(char *a, char *b) {
    return add_scaler_in(a, b, -1);
}

char *muls_in(char a, char *b) {
    size_t l = strlen(b), i;
    char *c = malloc(sizeof(char) * (l + 2));
    int carry, va = char2digit(a);
    for (i = 0, carry = 0; i != l; i++) {
        int v = carry + va * char2digit(b[i]);
        carry = v / 10;
        c[i] = digit2char(v % 10);
    }
    if (carry != 0) {
        c[i++] = digit2char(carry);
    }
    c[i] = '\0';
    return c;
}

char *shift_in(char *s, size_t n) {
    char *r = malloc(strlen(s) + n + 1);
    size_t i;
    for (i = 0; i != n; i++) {
        r[i] = '0';
    }
    strcpy(r + n, s);
    return r;
}

char *multiply_in(char *a, char *b) {
    size_t i, l = strlen(a);
    char *sum = strdup("");
    for (i = 0; i != l; i++) {
        // (a[0] + .. + a[i] * 10 ^ i + ..) * b
        // = a[0] * b + .. + a[i] * b * 10 ^ i + ..
        char *s0 = muls_in(a[i], b);
        char *s1 = shift_in(s0, i);
        char *s2 = add_in(sum, s1);
        free(s0);
        free(s1);
        free(sum);
        sum = s2;
    }
    return sum;
}

void halve(char *a, int l0, int l1, char **pa0, char **pa1) {
    char *a0 = malloc(sizeof(char) * (l0 + 1));
    char *a1 = malloc(sizeof(char) * (l1 + 1));
    strncpy(a0, a, l0);
    a0[l0] = '\0';
    strncpy(a1, a + l0, l1);
    a1[l1] = '\0';
    *pa0 = a0;
    *pa1 = a1;
}

char *multiply_rec(char *a, char *b) {
    int la = strlen(a);
    int lb = strlen(b);
    if (la < lb) {
        return multiply_rec(b, a);
    } else if (lb == 0) {
        return strdup("");
    } else if (la == 1) {
        char *c = malloc(3);
        int v = char2digit(a[0]) * char2digit(b[0]);
        int offset = 0;
        c[offset++] = digit2char(v % 10);
        if (v >= 10) {
            c[offset++] = digit2char(v / 10);
        }
        c[offset] = '\0';
        return c;
    } else if (la >= 2 * lb) {
        char *a0, *a1;
        halve(a, la - la / 2, la / 2, &a1, &a0);
        char *c0 = multiply_rec(a0, b);
        char *c1 = multiply_rec(a1, b);
        char *c2 = shift_in(c0, la - la / 2);
        char *sum = add_in(c2, c1);
        free(a0);
        free(a1);
        free(c0);
        free(c1);
        free(c2);
        return sum;
    } else {
        char *a0, *a1, *b0, *b1;
        int half = lb / 2;
        halve(a, half, la - half, &a1, &a0);
        halve(b, half, lb - half, &b1, &b0);
        // Karatsuba algo from O(N^2) to O(N^log(3)) = O(N^1.585)
        // (a0 *B  + a1) * (b0 * B + b1) = a0b0 * B2 + (a0b1 + a1b0) * B + a1b1
        char *t[10];
        char *a0b0 = multiply_rec(a0, b0);
        char *a1b1 = multiply_rec(a1, b1);
        // (a0 + a1)(b0 + b1) - a0b0 - a1b1
        t[0] = add_in(a0, a1);
        t[1] = add_in(b0, b1);
        t[2] = multiply_rec(t[0], t[1]);
        t[3] = sub_in(t[2], a0b0);
        t[4] = sub_in(t[3], a1b1);

        char *a0b0_ = shift_in(a0b0, half * 2);
        char *mid_ = shift_in(t[4], half);
        t[5] = add_in(a0b0_, mid_);
        char *ret = add_in(t[5], a1b1);
        free(a0);
        free(a1);
        free(b0);
        free(b1);
        free(a0b0);
        free(a1b1);
        free(a0b0_);
        free(mid_);
        int i;
        for (i = 0; i != 6; i++) {
            free(t[i]);
        }
#if 0
        char *tt = multiply_in(a, b);
        if (strcmp(tt, ret) != 0) {
            printf("calc %s*%s =%s != %s\n", a, b, ret, tt);
            // assert(0);
        }
        free(tt);
#endif
        return ret;
    }
}

char *multiply_mix(char *a, char *b) {
    int la = strlen(a);
    int lb = strlen(b);
    if (la < lb) {
        return multiply_mix(b, a);
    } else if (lb == 0 || la == 1 || la <= 2048) {
        // cross point is 2K~4K
        return multiply_in(a, b);
    } else if (la >= 2 * lb) {
        char *a0, *a1;
        halve(a, la - la / 2, la / 2, &a1, &a0);
        char *c0 = multiply_mix(a0, b);
        char *c1 = multiply_mix(a1, b);
        char *c2 = shift_in(c0, la - la / 2);
        char *sum = add_in(c2, c1);
        free(a0);
        free(a1);
        free(c0);
        free(c1);
        free(c2);
        return sum;
    } else {
        char *a0, *a1, *b0, *b1;
        int half = lb / 2;
        halve(a, half, la - half, &a1, &a0);
        halve(b, half, lb - half, &b1, &b0);
        // (a0 *B  + a1) * (b0 * B + b1) = a0b0 * B2 + (a0b1 + a1b0) * B + a1b1
        char *t[10];
        char *a0b0 = multiply_mix(a0, b0);
        char *a1b1 = multiply_mix(a1, b1);
        // (a0 + a1)(b0 + b1) - a0b0 - a1b1
        t[0] = add_in(a0, a1);
        t[1] = add_in(b0, b1);
        t[2] = multiply_mix(t[0], t[1]);
        t[3] = sub_in(t[2], a0b0);
        t[4] = sub_in(t[3], a1b1);

        char *a0b0_ = shift_in(a0b0, half * 2);
        char *mid_ = shift_in(t[4], half);
        t[5] = add_in(a0b0_, mid_);
        char *ret = add_in(t[5], a1b1);
        free(a0);
        free(a1);
        free(b0);
        free(b1);
        free(a0b0);
        free(a1b1);
        free(a0b0_);
        free(mid_);
        int i;
        for (i = 0; i != 6; i++) {
            free(t[i]);
        }
        return ret;
    }
}

/** 内部处理的格式为reverse(skipZeros(n)) 处理过后的数据
 * 也就是没有前导的零符号（直接使用空字符表示0）
 * 按照进位顺序排列的数字数组，该数组以'\0'为结束
 */
char *funcOnString(char *(f)(char *, char *), char *a, char *b) {
    // haskell style :)
    // (checkZero . reverse) .: f `on` (reverse . skipZeros)
    return checkZero(reverse(f(reverse(skipZeros(a)), reverse(skipZeros(b)))));
}

char *add(char *a, char *b) {
    a = strdup(a);
    b = strdup(b);
    char *c = funcOnString(add_in, a, b);
    free(a);
    free(b);
    return c;
}

char *muls(char a, char *b) {
    return checkZero(reverse(muls_in(a, reverse(skipZeros(b)))));
}

char *multiply_on_func(char *(*f)(char *, char *),
                       char *a, char *b) {
    // to accept string in static segment, duplicate it in enterance of code
    a = strdup(a);
    b = strdup(b);
    char *c = funcOnString(f, a, b);
    free(a);
    free(b);
    return c;
}

char *multiply(char *a, char *b) {
    return multiply_on_func(multiply_in, a, b);
}

char *multiply_rec_ex(char *a, char *b) {
    return multiply_on_func(multiply_rec, a, b);
}

char *multiply_mix_ex(char *a, char *b) {
    return multiply_on_func(multiply_mix, a, b);
}

// test code
#ifndef INCLUDED_MULTIPLYSTRING

bool unit_reverse(char *a, char *expect) {
    a = strdup(a);
    char *ret = reverse(a);
    bool r = strcmp(ret, expect) == 0;
    printf("reverse(%s) = %s %s\n", a, ret, expect);
    free(a);
    return r;
}

int test_reverse() {
    unit_reverse("", "");
    unit_reverse("1", "1");
    unit_reverse("12", "21");
    unit_reverse("123", "321");
    unit_reverse("1234", "4321");
    return 0;
}
char *symbol(bool r) {
    return r ? "==" : "/=";
}

bool unit(char *(*f)(char *, char *), char *funcName,
          const char *a, const char *b, const char *expect) {
    char *na = strdup(a), *nb = strdup(b);
    printf("%s(%s, %s) ====>\n", funcName, a, b);
    char *c = f(na, nb);
    bool r = strcmp(c, expect) == 0;
    printf("%s(%s, %s) = %s %s %s\n", funcName, a, b, c, symbol(r), expect);
    if (!r) {
        assert(0);
    }
    free(na);
    free(nb);
    free(c);
    return r;
}

bool test_func_commutative(char *(*f)(char *, char *), char *funcName,
                           const char *a, const char *b, const char *expect) {
    if (strcmp(a, b) == 0) {
        return unit(f, funcName, a, b, expect);
    } else {
        return unit(f, funcName, a, b, expect) && unit(f, funcName, b, a, expect);
    }
}

#define TEST_FUNC(f, a, b, c) test_func_commutative(f, #f, a, b, c)
#define TEST_ADD(a, b, c) TEST_FUNC(add, a, b, c)

bool test_add() {
    TEST_ADD("12", "3", "15");
    TEST_ADD("56", "789", "845");
    TEST_ADD("2", "0", "2");
    TEST_ADD("0", "0", "0");
    return true;
}

bool unit_muls(char a, const char *b, const char *expect) {
    char *nb = strdup(b);
    char *c = muls(a, nb);
    bool r = strcmp(c, expect) == 0;
    printf("muls(%c, %s) = %s %s %s\n", a, b, c, symbol(r), expect);
    free(c);
    free(nb);
    return r;
}

bool test_muls() {
    unit_muls('8', "123", "984");
    unit_muls('4', "123", "492");
    return true;
}

bool test_mul_func(char *(*f)(char *, char *), char *funcName) {
#define TEST_MUL(a, b, c) test_func_commutative(f, funcName, a, b, c)
    TEST_MUL("5", "13", "65");
    TEST_MUL("8", "18", "144");
    TEST_MUL("23", "56", "1288");
    TEST_MUL("123", "456", "56088");
    TEST_MUL("1234567890", "1234567890", "1524157875019052100");
    TEST_MUL("2", "3", "6");
    TEST_MUL("30", "69", "2070");
    TEST_MUL("11", "85", "935");
    TEST_MUL("2", "0", "0");
    TEST_MUL("0", "30", "0");
    TEST_MUL("0000001", "3", "3");
    TEST_MUL("1009", "03", "3027");
    TEST_MUL("98765", "56894", "5619135910");
    TEST_MUL("1020303004875647366210", "2774537626200857473632627613",
                          "2830869077153280552556547081187254342445169156730");
    TEST_MUL("1395", "5861209", "8176386555");
    TEST_MUL("58608473622772837728372827", "7586374672263726736374",
                          "444625839871840560024489175424316205566214109298");
    TEST_MUL("9007199254740991", "9007199254740991",
                          "81129638414606663681390495662081");
    return true;
}

char *str_init_rnd(int len) {
    char *na = malloc(sizeof(char) * (len + 1));
    int i;
    for (i = 0; i != len; i++) {
        na[i] = digit2char(rand() % 10);
    }
    na[len] = '\0';
    return na;
}

bool test_perf(char *(*f)(char *, char *), int len) {
    char *na = str_init_rnd(len), *nb = str_init_rnd(len);

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    char *nc = f(na, nb);

    gettimeofday(&stop, NULL);
    printf("%lu",
           (stop.tv_sec - start.tv_sec) * 1000 * 1000 +
           stop.tv_usec - start.tv_usec);

    // printf("mul(%s, %s) = %s\n", na, nb, nc);
    free(na);
    free(nb);
    free(nc);
    return true;
}

bool basic_group() {
    test_reverse();
    test_add();
    test_muls();
    test_mul_func(multiply, "mul");
    test_mul_func(multiply_rec_ex, "mul_rec");
    test_mul_func(multiply_mix_ex, "mul_mix");
    return true;
}

bool perf_group() {
    int i;
    for (i = 4; i != 64 * 1024; i *= 2) {
        printf("%d\t", i);
        test_perf(multiply, i);
        printf("\t");
        test_perf(multiply_rec_ex, i);
        printf("\t");
        test_perf(multiply_mix_ex, i);
        printf("\n");
    }
    return true;
}

int main() {
    basic_group();
    perf_group();
    return 0;
}
#endif
