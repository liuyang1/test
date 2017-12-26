#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX(a, b) (a > b ? a : b)
#define NUM 25
int prime[NUM] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
                  43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

void reverse(char *s) {
    int i, n = strlen(s);
    for (i = 0; i != n / 2; i++) {
        char c = s[i];
        s[i] = s[n - i - 1];
        s[n - i - 1] = c;
    }
}

char *add_in(char *a, char *b) {
    int la = strlen(a), lb = strlen(b);
    int lc = MAX(la, lb) + 2;
    char *c = malloc(sizeof(char) * lc);
    int i, carry;
    for (i = 0, carry = 0; i < la || i < lb; i++) {
        int va = i < la ? a[i] - '0' : 0;
        int vb = i < lb ? b[i] - '0' : 0;
        int v = va + vb + carry;
        c[i] = v % 10 + '0';
        carry = v / 10;
    }
    if (carry != 0) {
        c[i++] = carry + '0';
    }
    c[i] = '\0';
    return c;
}

char *muls_in(int a, char *b) {
    if (a == 0) {
        return strdup("");
    }
    if (a == 1) {
        return strdup(b);
    }
    char *d = muls_in(a / 2, b);
    char *r = add_in(d, d);
    free(d);
    if (a % 2 == 0) {
        return r;
    }
    char *r1 = add_in(r, b);
    free(r);
    return r1;
}

char *add(char *a, char *b) {
    reverse(a);
    reverse(b);
    char *c = add_in(a, b);
    reverse(a);
    reverse(b);
    reverse(c);
    return c;
}

char *muls(int a, char *b) {
    reverse(b);
    char *c = muls_in(a, b);
    reverse(b);
    reverse(c);
    return c;
}

char *cvtRadix(int *a, int n) {
    int i;
    char *sum, *radix;
    for (i = 0, sum = strdup("0"), radix = strdup("1"); i != n; i++) {
        char *r0 = muls(a[n - i - 1], radix);
        char *r1 = add(r0, sum);
        free(r0);
        free(sum);
        sum = r1; // sum += a[n - i - 1] * radix;
        char *r2 = muls(prime[i], radix);
        free(radix);
        radix = r2;
    }
    free(radix);
    return sum;
}

int main() {
    char c;
    int a[NUM];
    while (1) {
        int i;
        for (i = 0; i != NUM;) {
            if (scanf("%d", &a[i]) == EOF) {
                return 0;
            }
            i++;
            if ((c = getchar()) != ',') {
                break;
            }
        }
        if (i == 1 && a[0] == 0) {
            return 0;
        }
        char *c = cvtRadix(a, i);
        printf("%s\n", c);
        free(c);
    }
}
