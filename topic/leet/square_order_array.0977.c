#include <stdlib.h>
#include <stdio.h>

int *square_ordered_array(int *xs, size_t n) {
    int *ys = malloc(sizeof(int) * n);
    size_t i, j, k;
    for (i = 0, j = k = n - 1; i <= j; ) {
        int a = abs(xs[i]), b = abs(xs[j]);
        if (a < b) {
            ys[k--] = b * b;
            j--;
        } else {
            ys[k--] = a * a;
            i++;
        }
    }
    return ys;
}

void show_array(int *xs, size_t n) {
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%d, ", xs[i]);
    }
    printf("\n");
}

int main() {
    int xs[] = {-4, -1, 0, 3, 10};
    size_t n = sizeof(xs) / sizeof(xs[0]);
    int *ys = square_ordered_array(xs, n);
    show_array(ys, n);
    return 0;
}
