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

char *add_in(char *a, char *b) {
    size_t la = strlen(a), lb = strlen(b);
    size_t m = max(la, lb);
    char *c = malloc(sizeof(char) * (m + 2));
    size_t i;
    int carry;
    for (i = 0, carry = 0; i != m; i++) {
        int v, v0, v1;
        v0 = i < la ? char2digit(a[i]) : 0;
        v1 = i < lb ? char2digit(b[i]) : 0;
        v = carry + v0 + v1;
        carry = v / 10;
        c[i] = digit2char(v % 10);
    }
    if (carry != 0) {
        c[i++] = digit2char(carry);
    }
    c[i] = '\0';
    return c;
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

char *funcOnString(char *(f)(char *, char *), char *a, char *b) {
    // haskell style :)
    // (checkZero . reverse) .: f `on` (reverse . skipZeros)
    return checkZero(reverse(f(reverse(skipZeros(a)), reverse(skipZeros(b)))));
}

char *add(char *a, char *b) {
    return funcOnString(add_in, a, b);
}

char *muls(char a, char *b) {
    return checkZero(reverse(muls_in(a, reverse(skipZeros(b)))));
}

char *multiply_wrap(char *a, char *b) {
    return funcOnString(multiply_in, a, b);
}

char *multiply(char *a, char *b) {
    // to accept string in static segment, duplicate it in enterance of code
    a = strdup(a);
    b = strdup(b);
    char *c = multiply_wrap(a, b);
    free(a);
    free(b);
    return c;
}

// test code
#ifndef INCLUDED
char *symbol(bool r) {
    return r ? "==" : "/=";
}

bool unit_add(const char *a, const char *b, const char *expect) {
    char *na = strdup(a), *nb = strdup(b);
    char *c = add(na, nb);
    bool r = strcmp(c, expect) == 0;
    printf("add(%s, %s) = %s %s %s\n", a, b, c, symbol(r), expect);
    free(na);
    free(nb);
    free(c);
    return r;
}

bool test_func_commutative(bool (f)(const char *, const char *, const char *),
                           const char *a, const char *b, const char *expect) {
    if (strcmp(a, b) == 0) {
        return f(a, b, expect);
    } else {
        return f(a, b, expect) && f(b, a, expect);
    }
}

bool test_add() {
    test_func_commutative(unit_add, "12", "3", "15");
    test_func_commutative(unit_add, "56", "789", "845");
    test_func_commutative(unit_add, "2", "0", "2");
    test_func_commutative(unit_add, "0", "0", "0");
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

bool unit_mul(const char *a, const char *b, const char *expect) {
    char *na = strdup(a), *nb = strdup(b);
    char *c = multiply(na, nb);
    bool r = strcmp(c, expect) == 0;
    printf("mul(%s, %s) = %s %s %s\n", a, b, c, symbol(r), expect);
    free(na);
    free(nb);
    free(c);
    return true;
}

bool test_mul() {
    test_func_commutative(unit_mul, "123", "456", "56088");
    test_func_commutative(unit_mul, "1234567890", "1234567890", "1524157875019052100");
    test_func_commutative(unit_mul, "2", "3", "6");
    test_func_commutative(unit_mul, "30", "69", "2070");
    test_func_commutative(unit_mul, "11", "85", "935");
    test_func_commutative(unit_mul, "2", "0", "0");
    test_func_commutative(unit_mul, "0", "30", "0");
    test_func_commutative(unit_mul, "0000001", "3", "3");
    test_func_commutative(unit_mul, "1009", "03", "3027");
    test_func_commutative(unit_mul, "98765", "56894", "5619135910");
    test_func_commutative(unit_mul, "1020303004875647366210", "2774537626200857473632627613",
                          "2830869077153280552556547081187254342445169156730");
    test_func_commutative(unit_mul, "58608473622772837728372827", "7586374672263726736374",
                          "444625839871840560024489175424316205566214109298");
    test_func_commutative(unit_mul, "9007199254740991", "9007199254740991",
                          "81129638414606663681390495662081");
    return true;
}

int main() {
    test_add();
    test_muls();
    test_mul();
    return 0;
}
#endif
