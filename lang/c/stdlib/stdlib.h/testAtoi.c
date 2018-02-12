#include <stdio.h>
#include <stdlib.h>

int main() {
    char *ss[] = {
        "12345",
        "12344:124354",
        "1234M"
    };
    int i;
    for (i = 0; i != sizeof(ss) / sizeof(ss[0]); i++) {
        printf("%s -> %d\n", ss[i], atoi(ss[i]));
    }
    return 0;
}
