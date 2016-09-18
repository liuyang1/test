#include <stdio.h>
int main() {
    char *s = "hello, world!";
    printf("%s\n", s);

    // test with strings array
    char *ss[10] = {"hello", ",", "world", "!", NULL};
    char **p;
    for (p = ss; *p != NULL; p++) {
        printf("%s ", *p);
    }
    printf("\n");
    return 0;
}
