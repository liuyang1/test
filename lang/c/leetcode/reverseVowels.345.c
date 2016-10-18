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
int main() {
    char s[] = "hello";
    char *p = reverseVowels(s);
    EXPECT_EQ_STR(p, "holle");
    char s1[] = "leetcode";
    char *p1 = reverseVowels(s1);
    EXPECT_EQ_STR(p1, "leotcede");
    char s2[] = "aA";
    char *p2 = reverseVowels(s2);
    EXPECT_EQ_STR(p2, "aA");
    return 0;
}
