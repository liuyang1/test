#include <stdio.h>
#include <stdlib.h>

void free_buffer(char **buf) {
    if (*buf != NULL) {
        printf("free buf=%p\n", buf);
        free(*buf);
        *buf = NULL;
    } else {
        printf("no action on NULL buffer\n");
    }
}

void close_file(FILE **fp) {
    if (*fp) {
        printf("close fp=%p\n", *fp);
        fclose(*fp);
        *fp = NULL;
    } else {
        printf("no action on NULL fp\n");
    }
}

int main() {
    char *buf __attribute__ ((__cleanup__(free_buffer)))
        = malloc(sizeof(char) * 256);
    FILE *fp __attribute__((__cleanup__(close_file)));
    fp = fopen("testfile", "w");

    // when leave scope, it will clean up automatically
    return 0;
}
