#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

bool unit(const char *input, const char *delim, char *output[]) {
    assert(input);
    assert(delim);
    assert(output);
    char *str = strdup(input);

    // first call strtok_r with origin string pointer
    // next call with NULL, until return NULL.
    // use saveptr to ensure thread-safe
    char *s, *saveptr, *token;
    for (s = str;
         (token = strtok_r(s, delim, &saveptr)) && *output;
         s = NULL, output++) {
        if (strcmp(token, *output) != 0) {
            printf("token = \"%s\" != \"%s\"\n", token, *output);
        }
    }

    free(str);
    return *output == NULL && token == NULL;
}

#define CHECK(expr) { bool b = expr; \
                      if (!b) {printf("fail at %s\n", # expr); return -1;}}

int main() {
    // normal case
    CHECK(unit("Hello World", " ",
               (char *[]) {"Hello", "World", NULL}));
    // no delimiter case
    CHECK(unit("Hello", " ",
               (char *[]) {"Hello", NULL}));
    // delimiters, and duplicate delimiter in segment
    CHECK(unit("Hello, World !", ", ",
               (char *[]) {"Hello", "World", "!", NULL}));
    return 0;
}
