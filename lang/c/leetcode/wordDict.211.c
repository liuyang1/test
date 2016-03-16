#include "leet.h"

struct WordDictionary {
    char **lst;
    int len;
    int end;
};

#define INIT_LEN        1
#define STEP_LEN        5
/** Initialize your data structure here. */
struct WordDictionary *wordDictionaryCreate() {
    struct WordDictionary *wd = malloc(sizeof(struct WordDictionary));
    wd->lst = malloc(sizeof(char *) * INIT_LEN);
    wd->len = INIT_LEN;
    wd->end = 0;
    return wd;
}

/** Inserts a word into the data structure. */
void addWord(struct WordDictionary *wd, char *word) {
    if (wd->end == wd->len) {
        wd->len += STEP_LEN;
        reallocM(&(wd->lst), wd->len * sizeof(char *));
    }
    wd->lst[wd->end] = word;
    wd->end++;
}

/** Returns if the word is in the data structure. A word could
    contain the dot character '.' to represent any one letter. */
bool search(struct WordDictionary *wd, char *word) {
    int i;
    char *p, *w;
    bool r;
    for (i = 0; i != wd->end; i++) {
        for (p = word, w = wd->lst[i], r = true;
             *p != '\0' && *w != '\0';
             p++, w++) {
            if (*p == '.') {
                continue;
            } else if (*p != *w) {
                r = false;
                break;
            }
        }
        if (r == true && *p == '\0' && *w == '\0') {
            return true;
        }
    }
    return false;
}

/** Deallocates memory previously allocated for the data structure. */
void wordDictionaryFree(struct WordDictionary *wd) {
    free(wd->lst);
    free(wd);
}

/** test code */
bool oneCase(struct WordDictionary *wd, char *word, bool e) {
    bool r = search(wd, word);
    printf("search [%s] %s\n", word, expect(r == e));
    return r == e;
}

int case1() {
#define WORDLEN     16
    char ws[][WORDLEN] = {"ran", "rune", "runner", "runs",
                          "add", "adds", "adder", "addee"};
    char pos[][WORDLEN] = {"r.n", "ru.n.r", "add", "add.", "adde.",
                           "...s", "....e.", "......", "...n.r"};
    char neg[][WORDLEN] = {".......", ".an.", "ru.n.e"};

    struct WordDictionary *wd = wordDictionaryCreate();
    int i;
    for (i = 0; i != sizeof(ws) / sizeof(ws[0]); i++) {
        addWord(wd, ws[i]);
    }
    for (i = 0; i != sizeof(pos) / sizeof(pos[0]); i++) {
        oneCase(wd, pos[i], true);
    }
    for (i = 0; i != sizeof(neg) / sizeof(neg[0]); i++) {
        oneCase(wd, neg[i], false);
    }
    wordDictionaryFree(wd);
    return 0;
}

int main() {
    case1();
    return 0;
}
