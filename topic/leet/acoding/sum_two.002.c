#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}
void show(int *a, size_t n) {
    size_t i;
    for (i = 0; i != n ; i++) {
        printf("%d, ", a[i]);
    }
    printf("\n");
}

int find_sum_of_two(int *a, size_t n, int x) {
    if (n < 2) {
        return 0;
    }
    qsort(a, n, sizeof(int), cmp_int);
    int r = 0;
    size_t b, e;
    for (b = 0, e = n; b < e - 1; ) {
        int s = a[b] + a[e - 1];
        // printf("%d %d %d, %zu %zu\n", a[b], a[e-1], s, b, e-1);
        if (s < x) {
            b++;
        } else if (s == x) {
            printf("%d+%d=%d\n", a[b], a[e-1], x);
            b++;
            r = 1;
        } else {
            e--;
        }
    }
    return r;
}

int main() {
    int a[] = {5, 7, 1, 2, 8, 4, 3};
    size_t n = sizeof(a) / sizeof(a[0]);
    int r = find_sum_of_two(a, n, 10);
    printf("target sum: %d\n", r);
    r = find_sum_of_two(a, n, 19);
    printf("target sum: %d\n", r);
    return 0;
}
