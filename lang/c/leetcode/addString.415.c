#include "leet.h"
void reverse(char *s) {
    int n = strlen(s);
    int b, e;
    char t;
    for (b = 0, e = n - 1; b < e; b++, e--) {
        t = s[b];
        s[b] = s[e];
        s[e] = t;
    }
}

char *addStrings(char *s1, char *s2) {
    int n1 = strlen(s1), n2 = strlen(s2);
    char *r = malloc(sizeof(char) * (n1 + n2 + 1));
    int i, j, k;
    int carry, n;
    for (i = n1 - 1, j = n2 - 1, k = carry = 0;
         i >= 0 || j >= 0 || carry != 0;
         i--, j--, k++) {
        n = carry;
        n += i >= 0 ? s1[i] - '0' : 0;
        n += j >= 0 ? s2[i] - '0' : 0;
        carry = n / 10;
        r[k] = n % 10 + '0';
    }
    r[k] = '\0';
    reverse(r);
    return r;
}

int main() {
    char *r = addStrings("1", "9");
    printf("%s\n", r);
    free(r);
    return 0;
}
