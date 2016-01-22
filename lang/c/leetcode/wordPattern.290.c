#include "leet.h"

const int patternsetsz = 26;
bool anySame(char **ptnset, int sz, char *s) {
    int i;
    for (i = 0; i != sz; i++) {
        if (ptnset[i] == NULL) {
            continue;
        }
        if (strcmp(ptnset[i], s) == 0) {
            return true;
        }
    }
    return false;
}

bool wordPattern(char *pattern, char *str) {
    str = strdup(str);
    char **ptnset = malloc(sizeof(char *) * patternsetsz);
    memset(ptnset, 0, sizeof(char *) * patternsetsz);

    char *p, *s, *ptn;
    bool r = true;
    for (p = pattern, s = strtok(str, " "); *p != '\0' && s; p++, s = strtok(NULL, " ")) {
        ptn = ptnset[*p - 'a'];
        LOG("%c %d %p %s %s\n", *p, *p - 'a', ptn, ptn ? ptn : "", s);
        if (ptn == NULL) {
            if (anySame(ptnset, patternsetsz, s)) {
                r = false;
                goto final;
            }
            ptnset[*p - 'a'] = s;
        } else if (strcmp(ptn, s) != 0) {
            LOG("%s %s not same\n", ptn, s);
            r = false;
            goto final;
        }
    }
final:
    if (r == true) {
        r = *p == '\0' && s == NULL;
    }
    free(ptnset);
    free(str);
    return r;
}

#define CASE(sp, ss, e) {bool r = wordPattern(sp, ss); \
                         printf("%s - %s %s ?= %s %s\n", \
                                sp, ss, SBOOL(r), SBOOL(e), expect(r == e)); }

int main() {
    CASE("abba", "dog cat cat dog", true);
    CASE("abba", "dog cat cat fish", false);
    CASE("aaaa", "dog cat cat dog", false);
    CASE("abba", "dog dog dog dog", false);
    CASE("abba", "dog cat cat dot", false);
    CASE("jquery", "jquery", false);
    CASE("aaa", "cat cat", false);
    return 0;
}
