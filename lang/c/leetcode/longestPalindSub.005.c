#include <string.h>
#include "leet.h"

int indexMax(int *p, int n, int *pIdx) {
    assert(p != NULL);
    assert(n > 0);
    int i, m = p[0], mi = 0;
    for (i = 1; i != n; i++) {
        if (p[i] > m) {
            m = p[i];
            mi = i;
        }
    }
    *pIdx = mi;
    return m;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DELIMITER '#'
char *longestPalindrome(char *s) {
    int len = strlen(s);
    if (len <= 1) {
        return strdup(s);
    }
    int nlen = 2 * len + 1;
    char *ns = malloc(sizeof(char) * (nlen + 1));
    int *p = malloc(sizeof(int) * (nlen + 1));
    int i;
    for (i = 0; i != nlen; i++) {
        if (i % 2 == 0) {
            ns[i] = DELIMITER;
        } else {
            ns[i] = s[(i - 1) / 2];
        }
        p[i] = 0;
    }
    ns[nlen] = '\0';
    p[nlen] = 0;

    int mx = 0, cen = 0;
    for (i = 1; i != nlen; i++) {
        p[i] = mx > i ? MIN(p[2 * cen - i], mx - i) : 1;
        while (i + p[i] < nlen && i - p[i] >= 0 && ns[i - p[i]] == ns[i + p[i]]) {
            p[i] += 1;
        }
        if (i + p[i] > mx) {
            mx = i + p[i];
            cen = i;
        }
    }

    int idx;
    int radius = indexMax(p, nlen, &idx);
    radius = radius - 1;
    char *r = malloc(sizeof(char) * (radius + 2));
    int j;
    for (i = idx - radius, j = 0; i != idx + radius + 1 && j != radius + 1; i++) {
        if (ns[i] != DELIMITER) {
            r[j] = ns[i];
            j++;
        }
    }
    r[j] = '\0';

    free(ns);
    free(p);
    return r;
}

#define CHECK(s, e) {char *rt = longestPalindrome(s);                              \
                     if (rt == NULL) {                                             \
                         printf("testLongestPalindrome(\'%s\') == NULL %s\n",      \
                                s, expect(false));                                 \
                         return false;                                             \
                     }                                                             \
                     if (strcmp(rt, e) != 0) {                                     \
                         printf("longestPalindrome(\'%s\')=\'%s\' != \'%s\' %s\n", \
                                s, rt, e, expect(false));                          \
                         free(rt);                                                 \
                         return false;                                             \
                     }                                                             \
                     free(rt);                                                     \
}
bool testLongestPalindrome() {
    bool r = true;
    CHECK("", "");
    CHECK("a", "a");
    CHECK("aa", "aa");
    char s0[] = "abcdefedcba";
    CHECK(s0, s0);
    char s1[] = "sator arepo tenet opera rotas";
    CHECK(s1, s1);
    char s2[] = "detartrated";
    CHECK(s2, s2);
    printf("all cases %s\n", expect(r));
    return r;
}

#define LEN 10000
bool testPerf() {
    bool r = true;
    char *s = malloc(sizeof(char) * LEN);
    int i;
    for (i = 0; i != LEN; i++) {
        s[i] = 'a';
    }
    CHECK(s, s);
    free(s);
    return true;
}

int main() {
    testLongestPalindrome();
    testPerf();
    return 0;
}
