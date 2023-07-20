#include <stdio.h>

void reversed(int *a, int *b) {
    for (; a < b; a++, b--) {
        int t = *a;
        *a = *b;
        *b = t;
    }
}

void rotated(int *a, size_t n, size_t offset) {
    reversed(a, a + offset);
    reversed(a + offset + 1, a + n - 1);
    reversed(a, a + n - 1);
}

void show(int *a, size_t n) {
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%d, ", a[i]);
    }
    printf("\n");
}

int pivot(int *a, size_t n) {
    size_t b, e;
    for (b = 0, e = n - 1; b != e; ) {
        if (a[b] < a[e]) {
        }
    }

}

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7};
    size_t n = sizeof(a) / sizeof(a[0]);
    rotated(a, n, 3);
    show(a, n);
    return 0;
}
