#include "leet.h"

typedef struct Stat {
    int freq;
    int firstIdx;
} Stat;

#define ALPHATBLLEN (26)
int firstUniqChar(char *s) {
    Stat st[ALPHATBLLEN] = {{0, 0}};
    char *p;
    int i, ti, len;
    for (i = 0, p = s; *p; i++, p++) {
        ti = *p - 'a';
        st[ti].freq++;
        if (st[ti].freq == 1) {
            st[ti].firstIdx = i;
        }
    }
    len = i;
    for (i = 0, ti = len; i != ALPHATBLLEN; i++) {
        if (st[i].freq == 1 && ti > st[i].firstIdx) {
            ti = st[i].firstIdx;
        }
    }
    if (ti == len) {
        ti = -1;
    }
    return ti;
}

bool unit(char *s, int e) {
    int r = firstUniqChar(s);
    if (r != e) {
        printf("firstUniqChar(\'%s\') = %d != %d %s\n", s, r, e, expect(false));
        return false;
    }
    return true;
}

int main() {
    unit("leetcode", 0);
    unit("loveleetcode", 2);
    unit("leetteel", -1);
    unit("itwqbtcdprfsuprkrjkausiterybzncbmdvkgljxuekizvaivszowqtmrttiihervpncztuoljftlxybpgwnjb",
         61);
    return 0;
}
