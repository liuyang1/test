#include <stdlib.h>
#include <stdio.h>

size_t index_sorted(int x, int *a, size_t l, size_t h) {
    size_t m = (l + h) / 2;
    if (x == a[m]) {
        return m;
    } else if (x > a[m]) {
        return index_sorted(x, a, m+1, h);
    } else {
        return index_sorted(x, a, l, m-1);
    }
}

size_t index_rotated(int x, int *a, size_t l, size_t h) {
    size_t m = (l + h) / 2;
    // printf("x=%d %zu %zu %zu, %d %d %d\n", x, l, m, h, a[l], a[m], a[h]);
    if (x == a[l]) {
        return l;
    } else if (x == a[h]) {
        return h;
    } else if (x == a[m]) {
        return m;
    }  else if (a[m] < a[h]) {
        if (x < a[m] || x > a[l]) { // x?? < a[m] < a[h] < a[l] < x??
            return index_rotated(x, a, l, m-1);
        } else {
            return index_sorted(x, a, m+1, h);
        }
    } else if (a[m] > a[h]) {
        if (x > a[m] || x < a[h]) { // x?? < a[h] < a[l] < a[m] < x??
            return index_rotated(x, a, m+1, h);
        } else {
            return index_sorted(x, a, l, m-1);
        }
    } else if (a[m] == a[h]) {
        return index_rotated(x, a, l, m-1);
    }
}


int main() {
    int a[] = {176, 188, 199, 200, 210, 222, 1, 10, 20, 47, 59, 63, 75, 88, 99, 107, 120, 133, 155, 162};
    size_t n = sizeof(a) / sizeof(a[0]);
    size_t i;
    for (i = 0; i != n; i++) {
        printf("test on i=%zu %d\n", i, a[i]);
        size_t idx = index_rotated(a[i], a, 0, n - 1);
        if (idx != i) {
            printf("i=%zu %d -> %zu fail\n", i, a[i], idx);
        }
    }
    return 0;
}
