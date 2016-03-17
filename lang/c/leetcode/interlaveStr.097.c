#include "leet.h"

// should different with false 0 or true 1
#define UNDEF 2

bool isInterleaveI(int *mem, int m, int n, char *s1, char *s2, char *s3,
                   int csr1, int csr2);
bool isInterleaveM(int *mem, int m, int n, char *s1, char *s2, char *s3,
                   int csr1, int csr2) {
    int csr = csr1 * n + csr2;
    if (mem[csr] == UNDEF) {
        mem[csr] = isInterleaveI(mem, m, n, s1, s2, s3, csr1, csr2);
    }
    return mem[csr];
}

bool isInterleaveI(int *mem, int m, int n, char *s1, char *s2, char *s3,
                   int csr1, int csr2) {
    char c3 = s3[csr1 + csr2];
    if (c3 == '\0') {
        return true;
    }
    bool b1 = c3 == s1[csr1];
    bool b2 = c3 == s2[csr2];
    if (b1 && b2) {
        return isInterleaveM(mem, m, n, s1, s2, s3, csr1 + 1, csr2) ||
               isInterleaveM(mem, m, n, s1, s2, s3, csr1, csr2 + 1);
    } else if (b1) {
        return isInterleaveM(mem, m, n, s1, s2, s3, csr1 + 1, csr2);
    } else if (b2) {
        return isInterleaveM(mem, m, n, s1, s2, s3, csr1, csr2 + 1);
    } else {
        return false;
    }
}

bool isInterleave(char *s1, char *s2, char *s3) {
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int l3 = strlen(s3);
    if (l1 + l2 != l3) {
        return false;
    }
    int sz = (l1 + 1) * (l2 + 1);
    int *mem = malloc(sizeof(int) * sz);
    int i;
    for (i = 0; i != sz; i++) {
        mem[i] = UNDEF;
    }
    bool r = isInterleaveM(mem, l1 + 1, l2 + 1, s1, s2, s3, 0, 0);
    free(mem);
    return r;
}

#define CASE(s1, s2, s3, e) {bool r = isInterleave(s1, s2, s3); \
                             printf("[%s] [%s] [%s]\n%s ?= %s %s\n", \
                                    s1, s2, s3, SBOOL(r), SBOOL(e), expect(r == e)); }
int main() {
    CASE("", "", "", true);
    CASE("", "b", "b", true);
    CASE("aa", "bb", "abab", true);
    CASE("ac", "bd", "abcd", true);
    CASE("aabcc", "dbbca", "aadbbcbcac", true);
    CASE("aabcc", "dbbca", "aadbbbaccc", false);
    CASE(
        "bbbbbabbbbabaababaaaabbababbaaabbabbaaabaaaaababbbababbbbbabbbbababbabaabababbbaabababababbbaaababaa",
        "babaaaabbababbbabbbbaabaabbaabbbbaabaaabaababaaaabaaabbaaabaaaabaabaabbbbbbbbbbbabaaabbababbabbabaab",
        "babbbabbbaaabbababbbbababaabbabaabaaabbbbabbbaaabbbaaaaabbbbaabbaaabababbaaaaaabababbababaababbababbbababbbbaaaabaabbabbaaaaabbabbaaaabbbaabaaabaababaababbaaabbbbbabbbbaabbabaabbbbabaaabbababbabbabbab",
        false);
    return 0;
}
