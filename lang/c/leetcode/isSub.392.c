#include "leet.h"

bool isSubsequence(char *s, char *t) {
    for (; *s != '\0' && *t != '\0'; t++) {
        if (*s == *t) {
            s++;
        }
    }
    return *s == '\0';
}

bool unit(char *s, char *t, bool e) {
    bool r = isSubsequence(s, t);
    bool ret = r == e;
    if (!ret) {
        printf("isSubsequence(%s, %s) == %s != %s %s\n",
               s, t, SBOOL(r), SBOOL(e), expect(ret));
    }
    return ret;
}

int main() {
    unit("ace", "abcdef", true);
    unit("aec", "abcdef", false);
    unit("abc", "ahbgdc", true);
    unit("axc", "ahbgdc", false);
    return 0;
}
