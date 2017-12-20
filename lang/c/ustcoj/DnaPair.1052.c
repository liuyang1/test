#include <stdio.h>
#include <stdlib.h>

char *pairDna(char *p) {
    char *s = p;
    for (; *s != '\0'; s++) {
        switch (*s) {
            case 'A': *s = 'T'; break;
            case 'T': *s = 'A'; break;
            case 'G': *s = 'C'; break;
            case 'C': *s = 'G'; break;
            default: break;
        }
    }
    return p;
}
int main() {
    char *p = NULL;
    size_t m = 0;
    ssize_t n = 0;
    while ((n = getline(&p, &m, stdin)) > 0) {
        printf("%s", pairDna(p));
    }
    if (p != NULL) {
        free(p);
    }
    return 0;
}
