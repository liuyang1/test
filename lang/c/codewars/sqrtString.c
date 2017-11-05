#define INCLUDED_DIVIDESTRING
#include "divideString.c"
#undef INCLUDED_DIVIDESTRING

char *bound(char *n) {
    char *i = strdup("1");
    while (1) {
        char *i2 = multiply_in(i, i);
        int cmp = compare(i2, n);
        free(i2);
        if (cmp > 0) {
            break;
        }
        char *j = multiply_in(i, "2");
        free(i);
        i = j;
    }
    return i;
}

void shrink(char *n, char *a, char *b, char **pa, char **pb) {
    char *sum = add_in(a, b);
    char **r = divide_strings_in(sum, "2");
    char *mid = r[0];
    char *mid2 = multiply_in(mid, mid);
    int cmp = compare(mid2, n);
    free(sum);
    free(r[1]);
    free(r);
    free(mid2);
    if (cmp == 0) {
        *pa = *pb = mid;
        free(a);
        free(b);
        return;
    } else if (cmp > 0) {
        *pa = a;
        *pb = mid;
        free(b);
        return;
    } else {
        *pa = mid;
        *pb = b;
        free(a);
        return;
    }
}

char *sqrt_string_in(char *n) {
    char *b = strdup("0");
    char *e = bound(n);
    while (1) {
        shrink(n, b, e, &b, &e);
        char *sum = add_in(b, "1");
        bool cmp = compare(b, e) <= 0 && compare(sum, e) >= 0;
        free(sum);
        if (cmp) {
            if (b != e) {
                free(e);
            }
            return b;
        }
    }
}

char *sqrt_string(char *n) {
    char *nn = strdup(n);
    char *r = sqrt_string_in(reverse(skipZeros(nn)));
    r = checkZero(reverse(r));
    free(nn);
    return r;
}

char *integerSquareRoot(char *n) {
    return sqrt_string(n);
}

int compare_ex(char *a, char *b) {
    char *na = strdup(a);
    char *nb = strdup(b);
    int r = compare(reverse(skipZeros(na)), reverse(skipZeros(nb)));
    free(na);
    free(nb);
    return r;
}

// test code
bool unit_sqrt(char *n) {
    char *sqr = sqrt_string(n);
    char *m0 = multiply(sqr, sqr);
    char *a0 = add(sqr, "1");
    char *m1 = multiply(a0, a0);
    bool r = compare_ex(m0, n) <= 0 && compare_ex(n, m1) < 0;
    printf("sqrt(%s) = %s %s\n", n, sqr, r ? "SUCC" : "FAIL");
    if (!r) {
        assert(0);
    }
    free(m0);
    free(a0);
    free(m1);
    free(sqr);
    return r;
}

int test_simple_sqrt() {
    char s[1024];
    int i;
    for (i = 0; i != 200; i++) {
        sprintf(s, "%d", i);
        unit_sqrt(s);
    }
    return 0;
}

int test_large_sqrt() {
    unit_sqrt("23232328323215435345345345343458098856756556809400840980980980980809092343243243243243098799634");
    return 0;
}

int main() {
    test_simple_sqrt();
    test_large_sqrt();
    return 0;
}
