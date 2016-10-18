#include "leet.h"

void statString(char *s, int *n) {
    for (; *s; s++) {
        n[*s - 'a']++;
    }
}
#define NUM 26
bool canConstruct(char *note, char *magz) {
    int notest[NUM] = {0}, magzst[NUM] = {0};
    statString(note, notest);
    statString(magz, magzst);
    int i;
    for (i = 0; i != NUM; i++) {
        if (notest[i] > magzst[i]) {
            return false;
        }
    }
    return true;
}

int unit(char *n, char *m, bool e) {
    bool r = canConstruct(n, m);
    if (r != e) {
        printf("canConstruct(%s, %s) = %s != %s %s\n", n, m, SBOOL(r), SBOOL(e), expect(false));
    }
    return true;
}

int main() {
    unit("a", "b", false);
    unit("aa", "ab", false);
    unit("aa", "aab", true);
    return 0;
}
