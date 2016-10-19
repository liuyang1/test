#include "leet.h"
void reverse(char *s, int len) {
    int b, e;
    char t;
    for (b = 0, e = len - 1; b < e; b++, e--) {
        t = s[b];
        s[b] = s[e];
        s[e] = t;
    }
}

char digit2hex(int n) {
    if (n < 10) {
        return n + '0';
    } else {
        return n - 10 + 'a';
    }
    return 'x';
}

char *toHex(int num) {
    char *r = malloc(sizeof(char) * 9);
    unsigned int n = (unsigned int)num;
    int i, rem;
    for (i = 0; i < 8 && n != 0; i++) {
        r[i] = digit2hex(n % 16);
        n /= 16;
    }
    if (i == 0) {
        r[i++] = '0';
    }
    r[i] = '\0';
    reverse(r, i);
    return r;
}

int main() {
    char *p;
    p = toHex(26);
    printf("%s\n", p);
    p = toHex(-1);
    printf("%s\n", p);
    p = toHex(0);
    printf("%s\n", p);
    return 0;
}
