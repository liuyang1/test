/**
 * simple RE: regular expression matching
 * support:
 * - '.'    to match any single char
 * - c'*'   char c, repeat zero or any times
 * - c      match char c
 *
 * Solution:
 * - create Pattern List
 * - the key is to support ASTERISK to match zero or anytimes:
 *      use recursive call to implement it.
 */
#include "leet.h"

// use linked list to store pattern
typedef struct pat_t {
    enum {
        RAWCHAR = 0,
        ANYSINGLECHAR = 1,
        ANYREPEATCHAR = 2,
    } type;
    char c;
    struct pat_t *next;
} pat;


pat *createPat(char *p) {
    pat *head = NULL, *rr = NULL;;
    for (; *p != '\0'; p++) {
        pat *r = malloc(sizeof(pat));
        if (p[1] == '*') {
            r->type = ANYREPEATCHAR;
            r->c = *p;
            p++;
        } else if (*p == '.') {
            r->type = ANYSINGLECHAR;
        } else {
            r->type = RAWCHAR;
            r->c = *p;
        }
        if (!head) {
            head = r;
            rr = head;
        } else {
            rr->next = r;
            rr = r;
        }
    }
    if (rr != NULL) {
        rr->next = NULL;
    }
    return head;
}

void showPat(pat *p) {
    if (p == NULL) {
        printf("nil\n");
    } else {
        switch (p->type) {
            case RAWCHAR: printf("%c ", p->c); break;
            case ANYSINGLECHAR: printf(" . "); break;
            case ANYREPEATCHAR: printf(" %c* ", p->c); break;
            default: printf(" UNKNOWN "); break;
        }
        showPat(p->next);
    }
}

void destroyPat(pat *p) {
    for (; p != NULL;) {
        pat *next = p->next;
        free(p);
        p = next;
    }
}

bool matchPat(pat *p, char *s) {
    if (p == NULL) {
        return *s == '\0';
    }
    if (*s == '\0') {
        return p->type == ANYREPEATCHAR && matchPat(p->next, s);
    }
    switch (p->type) {
        case RAWCHAR:
            return p->c == *s && matchPat(p->next, s + 1);
        case ANYSINGLECHAR:
            return matchPat(p->next, s + 1);
        case ANYREPEATCHAR:
            return matchPat(p->next, s) ||
                   ((p->c == '.' || p->c == *s) && matchPat(p, s + 1));
        default:
            return false;
    }
}

bool isMatch(char *s, char *p) {
    pat *pt = createPat(p);
    bool r = matchPat(pt, s);
    destroyPat(pt);
    return r;
}

/** test code */
bool unit(char *s, char *p, bool e) {
    bool r = isMatch(s, p);
    bool ret = r == e;
    printf("isMatch(%s, %s) == %s ?= %s >>> %s\n",
           s, p, SBOOL(r), SBOOL(e), expect(ret));
    return ret;
}

bool testPat() {
    pat *p = createPat("abcdef.*c*.aaa");
    showPat(p);
    destroyPat(p);
    return true;
}

bool test() {
    bool r = true;
    r &= unit("aa", "a", false);
    r &= unit("aa", "aa", true);
    r &= unit("aaa", "aa", false);
    r &= unit("aa", "a*", true);
    r &= unit("aa", ".*", true);
    r &= unit("ab", ".*", true);
    r &= unit("aab", "c*a*b", true);

    r &= unit("aaa", "a*", true);
    r &= unit("aaa", "aaaa", false);
    r &= unit("aaaaaa", "a*aa", true); // asterisk should not so aggressive
    r &= unit("aa", "a*aaa", false);
    r &= unit("aaaaaa", "a*bb", false);

    r &= unit("abbc", ".*bc", true);
    r &= unit("aacc", ".*bc", false);
    r &= unit("cc", ".*ccc", false);

    r &= unit("aaa", "ac*a*c*a", true);
    r &= unit("", "abc", false);
    r &= unit("a", "", false);

    return r;
}

int main() {
    testPat();
    test();
    return 0;
}
