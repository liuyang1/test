#include <alloca.h>
#include <stdio.h>
#include <string.h>

int main() {
    const char *s = "Hello, World!";
    size_t len = strlen(s) + 1;

    void *p = alloca(len);
    memset(p, '\n', len);
    memcpy(p, s, len);

    printf("origin:%s\n", s);
    printf("stack :%s\n", (char *)p);

    return 0;
}
