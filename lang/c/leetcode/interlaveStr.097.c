#include "leet.h"

// BAD performance as exponentail complexity
inline bool isInterleaveI(char *s1, char *s2, char *s3) {
    if (*s3 == '\0') {
        return true;
    }
    if (*s3 == *s1 && *s3 == *s2) {
        return isInterleaveI(s1 + 1, s2, s3 + 1) ||
               isInterleaveI(s1, s2 + 1, s3 + 1);
    } else if (*s3 == *s1) {
        return isInterleaveI(s1 + 1, s2, s3 + 1);
    } else if (*s3 == *s2) {
        return isInterleaveI(s1, s2 + 1, s3 + 1);
    } else {
        return false;
    }
}

bool isInterleave(char *s1, char *s2, char *s3) {
    if (strlen(s1) + strlen(s2) != strlen(s3)) {
        return false;
    }
    return isInterleaveI(s1, s2, s3);
}

#define CASE(s1, s2, s3, e) {bool r = isInterleave(s1, s2, s3); \
                             printf("%s %s %s\n%s ?= %s %s\n", \
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
