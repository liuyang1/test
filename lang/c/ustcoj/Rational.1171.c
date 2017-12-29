#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int myPow10(int e) {
    int i, r;
    for (i = 0, r = 1; i != e; i++) {
        r *= 10;
    }
    return r;
}

int cntDigit(int a) {
    int cnt;
    for (cnt = 0; a != 0; cnt++) {
        a /= 10;
    }
    return cnt;
}

int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    if (a > b) {
        return gcd(a % b, b);
    } else {
        return gcd(a, b % a);
    }
}

void convert(int a, int b, int la, int lb, int *num, int *den) {
    printf("arg: %d %d %d %d\n", a, b, la, lb);
    int g;
    if (b == 0) {
        *num = a;
        printf("arg: la=%d\n", la);
        *den = myPow10(la);
    } else if (a == 0) {
        *num = b;
        printf("arg: lb=%d\n", lb);
        *den = myPow10(lb) - 1;
    } else {
        printf("arg: la=%d lb=%d\n", la, lb);
        *num = (a * myPow10(lb) + b - a);
        *den = myPow10(la) * (myPow10(lb) - 1);
    }
    g = gcd(*num, *den);
    // printf("%d %d -> %d\n", *num, *den, g);
    *num /= g;
    *den /= g;
}

int main() {
    int i, casenum;
    scanf("%d", &casenum);
    getchar();
    for (i = 0; i != casenum; i++) {
        int a = 0, b = 0, den = 0, num = 0, la = -1, lb = -1;
        char *s = NULL;
        size_t len = 0;
        getline(&s, &len, stdin);
        if (s == NULL || len == 0) {
            break;
        }
        char *p0 = strchr(s, '.');
        if (p0 != NULL && p0[1] != '(') {
            a = atoi(p0 + 1);
        }
        char *p1 = strchr(s, '(');
        if (p1 != NULL) {
            b = atoi(p1 + 1);
        }
        // la = cntDigit(a);
        // lb = cntDigit(b);
        char *p2 = strchr(s, '\n');
        if (p1 != NULL) {
            la = p1 - p0 - 1;
        } else if (p2 != NULL) {
            la = p2 - p0 - 1;
        }
        lb = p2 - p1 - 2;
        convert(a, b, la, lb, &num, &den);
        // printf("%s -> %d/%d -> %lf\n", s, num, den, (num + 0.0) / (den + 0.0));
        printf("%d/%d\n", num, den);
        free(s);
    }
    return 0;
}
