#include <stdio.h>
#include <stdlib.h>

int main() {
    char *buf;
    buf = malloc(1 << 31);

    fgets(buf, 1024, stdin);
    printf("%s\n", buf);

    if (buf != NULL) {
        free(buf);
    }
    return 0;
}
