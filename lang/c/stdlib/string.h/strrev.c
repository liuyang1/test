/*
 * ========================================================
 *
 *       Filename:  strrev.c
 *
 *    Description:
 *
 *        Version:  1.0
 *                  1.1 add strrev_1
 *        Created:  2013年09月08日 20时38分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>
#include <string.h>

void strrev_1(char *p) {
    for (char *q = p + strlen(p) - 1; p < q; p++, q--) {
        char t = *p;
        *p = *q;
        *q = t;
    }
}

void strrev(char *p) {
    int n = strlen(p);
    for (int i = 0; i != n / 2; i++) {
        char t = p[i];
        p[i] = p[n - i - 1];
        p[n - i - 1] = t;
    }
    return;
}

int unit(void (*f)(char *), char *s) {
    f(s);
    printf("%s\n", s);
    return 0;
}

int test(void (*f)(char *)) {
    char s0[] = "";
    char s1[] = "abcdefg";
    char s2[] = "abcdef";
    unit(f, s0);
    unit(f, s1);
    unit(f, s2);
    return 0;
}

int main() {
    test(strrev);
    test(strrev_1);
    return 0;
}
