#include "leet.h"
#define CHARSET     26
typedef struct {
    char *s;
    int stat[CHARSET];
    int len;
} Word;

void initWord(Word *w, char *s) {
    w->s = s;
    w->len = strlen(s);
    memset(w->stat, 0, sizeof(int) * CHARSET);
    int j;
    for (j = 0; j != w->len; j++) {
        w->stat[s[j] - 'a'] = 1;
    }
}

bool NullIntersection(Word *w0, Word *w1) {
    if (w0->len == 0 || w1->len == 0) {
        return true;
    }
    int i;
    for (i = 0; i != CHARSET; i++) {
        if (w0->stat[i] && w1->stat[i]) {
            return false;
        }
    }
    return true;
}

int prod(Word *w0, Word *w1) {
    return w0->len * w1->len;
}

int maxProduct(char **words, int wordsSize) {
    int i, j, m = 0, r;
    Word *ws = malloc(sizeof(Word) * wordsSize);
    for (i = 0; i != wordsSize; i++) {
        initWord(ws + i, words[i]);
    }
    for (i = 0; i != wordsSize; i++) {
        for (j = i + 1; j != wordsSize; j++) {
            if (NullIntersection(ws + i, ws + j)) {
                r = prod(ws + i, ws + j);
                if (r > m) {
                    m = r;
                }
            }
        }
    }
    free(ws);
    return m;
}

#define CASE(ss, e) {int r = maxProduct(ss, sizeof(ss) / sizeof(char *)); \
                     printf("%d ?= %d %s\n", r, e, expect(r == e)); }
int main() {
    CASE(((char *[]) {"hello", "world"}), 0);
    CASE(((char *[]) {"abcw", "bza", "foo", "bar", "xtfn", "abcdef"}), 16);
    CASE(((char *[]) {"a", "ab", "abc", "d", "cd", "bcd", "abcd"}), 4);
    CASE(((char *[]) {"a", "aa", "aaa", "aaaa"}), 0);
    return 0;
}
