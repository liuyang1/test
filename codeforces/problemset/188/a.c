#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main() {
    char c;
    while ((c = getchar()) != '\n') {
        if (isupper(c)) {
            c = tolower(c);
        }
        char *p = strchr("aeiouy", c); // Y is vowel, too
        if (p != NULL) {
            continue;
        }
        printf(".%c", c);
    }
    printf("\n");
    return 0;
}
