#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** find x in sorted array (ascending order)
 * return index if found, otherwise -1
 */
int find(int *a, int n, int x) {
    // binary search by iterator on [b, e]
    int b, e, m;
    for (b = 0, e = n - 1; b <= e; ) {
        m = (b + e) / 2;
        if (x == a[m]) {
            return m;
        } else if (x < a[m]) {
            e = m - 1;
        } else {
            b = m + 1;
        }
    }
    return -1;
}

int cmp(const void *x, const void *y) {
    const int *a = x;
    const int *b = y;
    return (*a) - (*b);
}

void greater(int *a, int n) {
    int *b = malloc(sizeof(int) * n);
    memcpy(b, a, sizeof(int) * n);
    qsort(b, n, sizeof(int), cmp);
    int i;
    for (i = 0; i != n; i++) {
        int idx = find(b, n, a[i]);
        idx += 1; // next greater one
        if (idx == n) {
            printf("_ ");
        } else {
            printf("%d ", b[idx]);
        }
    }
    printf("\n");
    free(b);
}

int main() {
    int a[] = {6, 3, 9, 8, 10, 2, 1, 15, 7};
    greater(a, sizeof(a) / sizeof(a[0]));
    int b[] = {13, 6, 7, 12};
    greater(b, sizeof(b) / sizeof(b[0]));
    return 0;
}
