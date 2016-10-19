#include "leet.h"

bool isVowels(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

char *reverseVowels(char *s) {
    int n = strlen(s);
    int b, e;
    char t;
    for (b = 0, e = n - 1; b < e; b++, e--) {
        while (!isVowels(s[b]) && b < e) {
            b++;
        }
        while (!isVowels(s[e]) && b < e) {
            e--;
        }
        if (b >= e) {
            break;
        }
        t = s[b];
        s[b] = s[e];
        s[e] = t;
    }
    return s;
}

#define EXPECT_EQ_STR(a, b) {if (strcmp(a, b) != 0) {        \
                                 printf("%s != %s\n", a, b); \
                                 return -1; }}
#define UNIT(a, b) {char *p = strdup(a);        \
                    char *r = reverseVowels(p); \
                    EXPECT_EQ_STR(r, b);        \
                    free(p); }
int main() {
    UNIT("hello", "holle");
    UNIT("leetcode", "leotcede");
    UNIT("aA", "Aa");
    return 0;
}
