#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    const int *x = a, *y = b;
    return (*x > *y) - (*x < *y);
}

int median(int *a, int n) {
    // simple and stupid qsort.
    // It's O(nlogn). If only want median, It's O(n)
    qsort(a, n, sizeof(int), cmp);
    if (n % 2 == 0) {
        return (a[n / 2] + a[n / 2 - 1]) / 2;
    } else {
        return a[n / 2];
    }
}

int main() {
    while (1) {
        int num;
        scanf("%d", &num);
        if (num == 0) {
            break;
        }
        int *a = malloc(sizeof(int) * num), j;
        for (j = 0; j != num; j++) {
            scanf("%d", &a[j]);
        }
        printf("%d\n", median(a, num));
        free(a);
    }
    return 0;
}
