#include <stdio.h>

/** needle is all different char */
const char *sm_uniq(const char *t, const char *p) {
    const char *s, *q;
    for (s = t; *s != '\0'; s++) {
        for (q = p; *q != '\0' && *s != '\0'; q++, s++) {
            printf("%c %c\n", *s, *q);
            if (*q == *s) {
                if (*(q + 1) == '\0') {
                    return s;
                }
                continue;
            }
            break;
        }
    }
    return NULL;
}

int main() {
    const char t[] = "addadsbcdefadsfghk";
    const char p[] = "asdf";
    const char *r = sm_uniq(t, p);
    printf("%s\n", r);
    return 0;
}
