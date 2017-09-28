#include "leet.h"

char *longestCommonPrefix(char **strs, int strsSize) {
    char *r;
    if (strsSize <= 0) {
        r = strdup("");
        return r;
    }
    r = strdup(strs[0]);
    int i, j, len = strlen(r);
    for (j = 0; j != len; j++) {
        for (i = 1; i != strsSize; i++) {
            if (strs[i][j] == '\0' || r[j] != strs[i][j]) {
                r[j] = '\0';
                goto final;
            }
        }
    }
final:
    return r;
}

#define CASE(sa, e) {char *p = longestCommonPrefix(sa, sizeof(sa) / sizeof(char *)); \
                     printf("%s ?= %s %s\n", p, e, expect(strcmp(p, e) == 0));       \
                     free(p); }

int main() {
    CASE(((char *[]) {"abc", "xyz"}), "");
    CASE(((char *[]) {"abc", "alpha"}), "a");
    CASE(((char *[]) {"zyx", "zoo", "zeta"}), "z");
    CASE(((char *[]) {"easy", "ease", "ea"}), "ea");
    return 0;
}
