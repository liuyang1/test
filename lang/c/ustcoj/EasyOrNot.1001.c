// huge input, so use gets is WRONG.
// try to use getline, as it could allocate buffer auto
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *p = NULL;
    size_t m = 0;
    ssize_t n;
    while ((n = getline(&p, &m, stdin)) > 0) {
        printf("%s", p);
    }
    if (p != NULL) {
        free(p);
    }
    return 0;
}
