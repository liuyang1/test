#include <stdio.h>
#include <stdlib.h>

int test_width(size_t sz) {
    printf("%s %zu\n", __func__, sz);
    int *p = aligned_alloc(32, sz);
    int i;
    for (i = 0; i != sz /sizeof(int); i++) {
        p[i] = rand() % 2;
    }
    return 0;
}

int main() {
    test_width(32);
    test_width(64);
    test_width(128);
    test_width(256);
    return 0;
}
