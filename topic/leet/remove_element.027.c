#include <stdio.h>

size_t remove_element(int *xs, size_t n, int x) {
    size_t i, j;
    for (i = j = 0; i != n; i++) {
        if (xs[i] != x) {
            xs[j++] = xs[i];
        }
    }
    return j;
}

void show_array(int *xs, size_t n, const char *label) {
    printf("%s/%p-%zu=[", label, xs, n);
    size_t i;
    for (i = 0; i != n; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", xs[i]);
    }
    printf("]\n");
}


int main() {
    int a[] = {0, 1, 2, 2, 3, 0, 4, 2};
    size_t n = sizeof(a) / sizeof(a[0]);
    n = remove_element(a, n, 2);
    show_array(a, n, "ret");
    return 0;
}
