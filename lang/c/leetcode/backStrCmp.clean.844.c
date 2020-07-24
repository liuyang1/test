#include <stdbool.h>
#include <stdio.h>

static inline char *skipB(char *s, char *b, int *oc) {
    while (s >= b) {
        /** check s>=b first, then check on value */
        for (; s >= b && *s == '#'; (*oc)++, s--) {
        }
        if (*oc == 0) {
            break;
        }
        /** check s is not '#' */
        for (; s >= b && *oc > 0 && *s != '#'; (*oc)--, s--) {
        }
    }
    return s;
}

static inline char *tail(char *s) {
    char *p;
    for (p = s; *p != '\0'; p++) {
    }
    return p;
}

static inline bool valid(char *p0, char *b0, char *p1, char *b1) {
    return p0 >= b0 && p1 >= b1;
}

bool backspaceCompare(char *s0, char *s1) {
    char *p0, *p1;
    int c0, c1;
    for (p0 = tail(s0), p1 = tail(s1), c0 = c1 = 0; valid(p0, s0, p1, s1); p0--, p1--) {
        p0 = skipB(p0, s0, &c0);
        p1 = skipB(p1, s1, &c1);
        if (!valid(p0, s0, p1, s1)) {
            break;
        }
        if (*p0 != *p1) {
            return false;
        }
    }
    p0 = skipB(p0, s0, &c0);
    p1 = skipB(p1, s1, &c1);
    return p0 == s0 - 1 && p1 == s1 - 1;
}

static inline char *showBool(bool b) {
    return b ? "true" : "false";
}

int unit(char *s, char *t, bool expect) {
    bool r = backspaceCompare(s, t);
    printf("%s %s expect=%s ret=%s %s\n",
           s, t, showBool(expect), showBool(r), showBool(expect == r));
    return 0;
}

int main() {
#if 1
    unit("ab#c", "ad#c", true);
    unit("ab##", "c#d#", true);
    unit("a##c", "#a#c", true);
    unit("a#c", "b", false);
    unit("##c", "d##c", true);
    unit("mp", "mu#p", true);
    unit("rrmp", "rrmu#p", true);
    unit("wrrmp", "wrrmu#p", true);
    unit("ywrrmp", "ywrrmu#p", true);
    unit("xywrrmp", "xywrrmu#p", true);
    unit("bxj##tw", "bxo#j##tw", true);
#endif
    unit("nzp#o#g", "b#nzp#0#g", true);
    return 0;
}
