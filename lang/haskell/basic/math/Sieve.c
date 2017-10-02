#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
#define NUM (1024 * 1024 * 16)
    bool *a = malloc(sizeof(bool) * NUM);
    if (a == NULL) {
        fprintf(stderr, "fail to allocate buffer with NUM=%d\n", NUM);
        return -1;
    }
    int i, j;
    for (i = 0; i != NUM; i++) {
        a[i] = true;
    }
    a[0] = a[1] = false;

    for (i = 2; i * i < NUM; i++) {
        if (a[i]) {
            for (j = i * 2; j < NUM; j += i) {
                a[j] = false;
            }
        }
    }

    int last = 0;
    for (i = 2; i != NUM; i++) {
        if (a[i]) {
            last = i;
            printf("%d\n", i);
        }
    }
    free(a);
    return 0;
}
