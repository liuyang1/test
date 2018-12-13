#include "leet.h"

bool isPrefix(char *s, char *t) {
    for (; *s != '\0' && *t != '\0' && *s == *t; s++, t++) {
    }
    return *t == '\0';
}

#define INVALID_MATCH_IDX (-1)
int matchNext(char *s, char **dict, int wordDictSize, int i) {
    for (; i != wordDictSize; i++) {
        if (isPrefix(s, dict[i])) {
            return i;
        }
    }
    return INVALID_MATCH_IDX;
}

bool wordBreak(char *s, char **dict, int wordDictSize) {
    if (*s == '\0') {
        return true;
    }
    int i;
    for (i = 0; i != INVALID_MATCH_IDX;) {
        i = matchNext(s, dict, wordDictSize, i);
        if (i == INVALID_MATCH_IDX) {
            return false;
        }
        printf("s=%s try %s\n", s, dict[i]);
        if (wordBreak(s + strlen(dict[i]), dict, wordDictSize)) {
            return true;
        } else {
            i++;
        }
    }
    return false;
}

bool unit(char *s, char **dict, int size, bool e) {
    bool r = wordBreak(s, dict, size);
    bool ret = r == e;
    if (!ret) {
        printf("wordBreak(%s, ", s);
        showStr2(dict, size);
        printf(") = %s ?= %s %s\n", SBOOL(r), SBOOL(e), expect(ret));
    }
    return ret;
}

int main() {
    char *dict[] = {"leet", "code", "return", "bad", "adb", "dba", "aaa", "aaad", "adba"};
    unit("leetcode", dict, COUNT_OF(dict), true);

    unit("adbbadaaa", dict, COUNT_OF(dict), true);
    unit("adbaaaaadb", dict, COUNT_OF(dict), true);

    printf("test perf\n");

    char s[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    char *dict1[] = {"a","aa","aaa","aaaa","aaaaa","aaaaaa","aaaaaaa","aaaaaaaa","aaaaaaaaa","aaaaaaaaaa"};
    unit(s, dict1, COUNT_OF(dict1), false);


    return 0;
}
