#include "leet.h"

int say(char *o, char v, int cnt) {
    char *origin = o;
    int r = sprintf(o, "%d", cnt);
    o += r;
    *o = v;
    o++;
    return o - origin;
}

void iter(char *s, char *next) {
    char *p, v, *o;
    int c;
    for (p = s, o = next, c = 0; *p != '\0'; p++) {
        if (c == 0) {
            v = *p;
            c++;
        } else if (*p == v) {
            c++;
        } else {
            o += say(o, v, c);
            v = *p;
            c = 1;
        }
    }
    o += say(o, v, c);
    *o = '\0';
}

char *countAndSay(int n) {
    if (n <= 0) {
        return NULL;
    }
    char *s = malloc(sizeof(char) * 2), *next;
    s[0] = '1'; s[1] = '\0';
    int i, len;
    for (i = 1; i != n; i++) {
        len = strlen(s);
        // at most need 2 * N + 1 memory
        next = malloc(sizeof(char) * (2 * len + 1));
        iter(s, next);
        free(s);
        s = next;
    }
    return s;
}

int main() {
    int i;
    for (i = 0; i != 15; i++) {
        printf("%2d: %s\n", i, countAndSay(i));
    }
    return 0;
}
