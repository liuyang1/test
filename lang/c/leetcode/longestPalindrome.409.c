#include <string.h>
#include "leet.h"

#define HALFLEN 26
#define ALPHATBLLEN (HALFLEN * 2 + 1)

int char2idx(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 26;
    }
    printf("c=%d\n", c);
    assert(0);
    return ALPHATBLLEN - 1;
}

void statAlphaTbl(char *s, int *tbl) {
    for (; *s; s++) {
        tbl[char2idx(*s)]++;
    }
}

int longestPalindrome(char *s) {
    int st[ALPHATBLLEN] = {0};
    statAlphaTbl(s, st);
    int i, sum, odd;
    for (i = sum = odd = 0; i != ALPHATBLLEN - 1; i++) {
        sum += (st[i] / 2) * 2;
        if (st[i] % 2 != 0) {
            odd = 1;
        }
    }
    sum += odd;
    return sum;
}

#define CHECK(s, e) {size_t r = longestPalindrome(s);                        \
                     if (r != e) {                                           \
                         printf("longestPalindrome(\'%s\')=%ld != %ld %s\n", \
                                s, r, e, expect(false));                     \
                         return false;                                       \
                     }                                                       \
}
bool testLongestPalindrome() {
    bool r = true;
    CHECK("", 0lu);
    CHECK("a", 1lu);
    CHECK("aa", 2lu);
    char s0[] = "abcdefedcba";
    CHECK(s0, strlen(s0));
    char s1[] = "satorarepotenetoperarotas";
    CHECK(s1, strlen(s1));
    char s2[] = "detartrated";
    CHECK(s2, strlen(s2));
    printf("all cases %s\n", expect(r));
    return r;
}

int main() {
    testLongestPalindrome();
    return 0;
}
