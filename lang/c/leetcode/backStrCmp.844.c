#include <stdbool.h>
#include <stdio.h>

// #define DEBUG printf
#define DEBUG(...)
char *skipB(char *s, char *b, int *oc) {
    /** check s>=b first, then check on value */
    while (s >= b) {
        for (; s >= b && *s == '#'; (*oc)++, s--) {
            ;
        }
        DEBUG("acc # on %ld cnt=%d\n", s - b, *oc);
        if (*oc == 0) {
            break;
        }
        for (; s >= b && *oc > 0 && *s != '#'; (*oc)--, s--) {
            ;
        }
        DEBUG("del # on %ld cnt=%d\n", s - b, *oc);
    }
    DEBUG("skip %ld\n", s - b);
    return s;
}


bool backspaceCompare(char *s0, char *s1) {
    char *p0, *p1;
    for (p0 = s0; *p0 != '\0'; p0++) {
        ;
    }
    for (p1 = s1; *p1 != '\0'; p1++) {
        ;
    }
    int c0 = 0, c1 = 0;
    while (1) {
        DEBUG("%ld %ld\n", p0 - s0, p1 - s1);
        p0 = skipB(p0, s0, &c0);
        p1 = skipB(p1, s1, &c1);
        if (p0 < s0 || p1 < s1) {
            break;
        }
        DEBUG("cmp char %ld %ld\n", p0 - s0, p1 - s1);
        if (*p0 != *p1) {
            return false;
        }
        p0--;
        p1--;
        if (p0 < s0 || p1 < s1) {
            break;
        }
    }
    p0 = skipB(p0, s0, &c0);
    p1 = skipB(p1, s1, &c1);
    DEBUG("final %ld %ld\n", p0 - s0, p1 - s1);
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
