#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

int main() {
    char *s;
    int len = asprintf(&s, "Hello, %s\n", "World!");
    printf("len=%d s=%s\n", len, s);
    free(s);
    return 0;
}
