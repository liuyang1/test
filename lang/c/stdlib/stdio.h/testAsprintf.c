#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

/**
 * asprintf(char **strp, const char *fmt, ...);
 * vsaprintf(char **strp, const char *fmt, va_list ap);
 * These function allocated a string large enough to hold the output including NULL byte
 * return a pointer to it via first argument
 * calling side should free the allocated buffer.
 */
int main() {
    char *s;
    int len = asprintf(&s, "Hello, %s\n", "World!");
    printf("len=%d s=%s\n", len, s);
    free(s);
    return 0;
}
