#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *rle(char *s) {
    char *r = malloc(sizeof(char) * (strlen(s) + 2));
    char c = 0;
    int n = 0;
    char *p = r;
    for (; *s != '\0'; s++) {
        if (c == 0) {
            c = *s;
            n = 1;
        } else if (c == *s) {
            n++;
        } else {
            if (n == 1) {
                p += sprintf(p, "%c", c);
            } else {
                p += sprintf(p, "%d%c", n, c);
            }
            c = *s;
            n = 1;
        }
    }
    return r;
}

int main() {
    int n, i;
    scanf("%d", &n);
    getchar(); // eat newline
    char *p = NULL;
    size_t m = 0;
    for (i = 0; i != n; i++) {
        int l = getline(&p, &m, stdin);
        if (l <= 0) {
            break;
        }
        char *r = rle(p);
        printf("%s\n", r);
        free(r);
    }
    if (p != NULL) {
        free(p);
    }
    return 0;
}
