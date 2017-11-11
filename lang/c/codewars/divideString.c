#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INCLUDED_MULTIPLYSTRING
#include "multiplyString.c"
#undef INCLUDED_MULTIPLYSTRING

int sign(int n) {
    return (n > 0) - (n < 0);
}

int compare_in(char *a, char *b) {
    int i, last = 0;
    for (i = 0;; i++) {
        if (a[i] == '\0' && b[i] != '\0') {
            return -1;
        } else if (a[i] != '\0' && b[i] == '\0') {
            return 1;
        } else if (a[i] == '\0' && b[i] == '\0') {
            return last;
        } else {
            int new = sign(a[i] - b[i]);
            if (new != 0) {
                last = new;
            }
        }
    }
}

int compare(char *a, char *b) {
    char *na = strdup(a);
    char *nb = strdup(b);
    int cmp = compare_in(reverse(skipZeros(na)), reverse(skipZeros(nb)));
    free(na);
    free(nb);
    return cmp;
}

char *sub_in(char *x, char *y) {
    assert(compare_in(x, y) >= 0);
    size_t ly = strlen(y);
    char *c = malloc(sizeof(char) * (strlen(x) + 1));
    size_t i;
    int carry;
    for (i = carry = 0; x[i] != '\0'; i++) {
        int v, v0, v1;
        v0 = char2digit(x[i]);
        v1 = i < ly ? char2digit(y[i]) : 0;
        v = carry + v0 - v1;
        if (v < 0) {
            carry = -1;
            v += 10;
        } else {
            carry = 0;
        }
        c[i] = digit2char(v);
    }
    while (i >= 1 && c[i - 1] == '0') {
        i--;
    }
    c[i] = '\0';
    return c;
}

char **divide_strings_in(char *x, char *y) {
    char *y2 = multiply_in("2", y);
    char **r;
    if (compare_in(x, y2) >= 0) {
        r = divide_strings_in(x, y2);
        char *a = multiply_in("2", r[0]);
        free(r[0]);
        r[0] = a;
    } else {
        r = malloc(sizeof(char *) * 2);
        r[0] = strdup("");
        r[1] = strdup(x);
    }
    if (compare_in(r[1], y) >= 0) {
        char *a = add_in(r[0], "1");
        char *b = sub_in(r[1], y);
        free(r[0]);
        free(r[1]);
        r[0] = a;
        r[1] = b;
    }
    free(y2);
    return r;
}

char **divide_strings(char *a, char *b) {
    char *na = strdup(a);
    char *nb = strdup(b);
    char **bp = divide_strings_in(reverse(skipZeros(na)),
                                  reverse(skipZeros(nb)));
    bp[0] = checkZero(reverse(bp[0]));
    bp[1] = checkZero(reverse(bp[1]));
    free(na);
    free(nb);
    return bp;
}

#ifndef INCLUDED_DIVIDESTRING
/*************************  TEST CODE  ***************************************/

bool unit_compare(char *a, char *b, int expect) {
    int ret = compare(a, b);
    bool r = ret == expect;
    printf("compare(%s, %s) = %d %s %d\n",
           a, b, ret, r ? "==" : "!!!=", expect);
    return r;
}

bool unit_rev_commutative(char *a, char *b, int expect) {
    if (strcmp(a, b) == 0) {
        return unit_compare(a, b, 0);
    }
    return unit_compare(a, b, expect) && unit_compare(b, a, -1 * expect);
}

int test_compare_in() {
    unit_rev_commutative("00", "0", 0);
    unit_rev_commutative("00", "9", -1);
    unit_rev_commutative("1", "2", -1);
    unit_rev_commutative("1", "1", 0);
    unit_rev_commutative("10", "2", 1);
    unit_rev_commutative("1234", "1234", 0);
    unit_rev_commutative("12", "10", 1);
    unit_rev_commutative("12", "21", -1);
    return 0;
}

bool unit_sub_in(char *x, char *y, char *expect) {
    char *ret = sub_in(x, y);
    bool r = strcmp(ret, expect) == 0;
    printf("sub_in(%s, %s) = %s %s %s\n",
           x, y, ret, r ? "==" : "!!!=", expect);
    free(ret);
    return r;
}

int test_sub_in() {
    unit_sub_in("12", "9", "21");
    unit_sub_in("12", "0", "12");
    unit_sub_in("12", "1", "02");
    unit_sub_in("00001", "1", "9999");
    unit_sub_in("36762", "42662", "931");
    unit_sub_in("9999", "9999", "");
    return 0;
}

bool unit_divide(char *x, char *y) {
    char **ret = divide_strings(x, y);
    char *m0 = multiply(ret[0], y);
    char *a0 = add(m0, ret[1]);
    bool r = compare(ret[1], y) < 0 && compare(a0, x) == 0;
    printf("unit_divide %s / %s = %s, %s %s\n",
           x, y, ret[0], ret[1], r ? "SUCC" : "FAIL");
    free(a0);
    free(m0);
    free(ret[0]);
    free(ret[1]);
    free(ret);
    return r;
}

int test_divide() {
    unit_divide("12", "5");
    unit_divide("123847897", "13245987");
    unit_divide("1", "13245987");
    unit_divide("1232398475", "13");
    unit_divide("0", "4");
    unit_divide("10", "2");
    return 0;
}

int main() {
    test_sub_in();
    test_compare_in();
    test_divide();
    return 0;
}
#endif
