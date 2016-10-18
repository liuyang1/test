#include "leet.h"

char *reverseString(char *s) {
    int n = strlen(s);
    int b, e;
    char t;
    for (b = 0, e = n - 1; b < e; b++, e--) {
        t = s[b];
        s[b] = s[e];
        s[e] = t;
    }
    return s;
}

int main() {
    char s[] = "helo";
    char *p = reverseString(s);
    printf("%s\n", p);
    return 0;
}
