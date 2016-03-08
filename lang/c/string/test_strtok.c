#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main() {
    // const string cannot change, so must duplicate it to heap
    char *str = strdup("hello\nworld\n");
    char delim[] = "\n";
    char *s, *saveptr, *token;

    // first call strtok_r with origin string pointer
    // next call with NULL, until return NULL.
    for (s = str; ; s = NULL) {
        // use saveptr to ensure thread-safe
        token = strtok_r(s, delim, &saveptr);
        if (token == NULL) {
            break;
        }
        printf("%s\n", token);
    }

    free(str);
    return 0;
}
