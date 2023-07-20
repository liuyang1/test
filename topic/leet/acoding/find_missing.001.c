#include <stdio.h>
// number [1, n+2], find missing one
// 3 7 1 2 8 4 5, size is 7, find missing 6
int find_missing(int *a, size_t n) {
    int x = 0;
    size_t i;
    for (i = 1; i != n + 2; i++) {
        x ^= i;
    }
    for (i = 0; i != n; i++) {
        x ^= a[i];
    }
    return x;
}

int main() {
    int a[] = {3, 7, 1, 2, 8, 4, 5};
    printf("missing: %d\n", find_missing(a, sizeof(a)/ sizeof(a[0])));
    return 0;
}
