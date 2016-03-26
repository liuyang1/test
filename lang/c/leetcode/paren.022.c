#include "leet.h"

int catalan(int n) {
    return comb(2 * n, n) / (n + 1);
}

int paren(char **p, int n, int l, int r) {
    if (l == 0 && r != 0) {
        int i;
        for (i = r; i != 0; i--) {
            p[0][2 * n - l - i] = ')';
        }
        r = 0;
    }
    if (r == 0) {
        p[0][2 * n - l - r] = '\0';
        return 1;
    }
    int c = 0, ret, i;
    if (l != 0) {
        ret = paren(p, n, l - 1, r);
        for (i = 0; i != ret; i++) {
            p[i][2 * n - l - r] = '(';
        }
        c += ret;
    }
    if (r > l) {
        ret = paren(p + c, n, l, r - 1);
        for (i = 0; i != ret; i++) {
            p[c + i][2 * n - l - r] = ')';
        }
        c += ret;
    }
    return c;
}

char** generateParenthesis(int n, int* returnSize) {
    int len = catalan(n), i;
    char **p = malloc(sizeof(char *) * len);
    for (i = 0; i != len; i++) {
        p[i] = malloc(sizeof(char) * (2 * n + 1));
    }
    paren(p, n, n, n);
    *returnSize = len;
    return p;
}

int testParen(int n) {
    int sz;
    char **p = generateParenthesis(n, &sz);
    printf("sz=%d\n", sz);
    int i;
    for (i = 0; i != sz; i++) {
        printf("%3d:%s\n", i, p[i]);
    }
    return sz;
}

int main() {
    testParen(5);
    return 0;
}
