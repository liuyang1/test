#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void *id(void *x) {
    return x;
}

int idInt(int x) {
    return x;
}

void *idRaw(void *x, size_t len) {
    void *p = malloc(len);
    memcpy(p, x, len);
    return p;
}

int main() {
    int a = 42;
    int b;
    b = *(int *)id(&a);
    printf("id(%d) = %d\n", a, b);

    int c = idInt(a);
    printf("idInt(%d) = %d\n", a, c);

    int d;
    void *p = idRaw(&a, sizeof(int));
    d = *(int *)p;
    printf("idRaw(%d) = %d\n", a, d);
    return 0;
}
