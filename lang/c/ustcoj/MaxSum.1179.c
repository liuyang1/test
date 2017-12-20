#include <stdio.h>
#include <stdlib.h>

int show(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
int maxsum(int *a, int n) {
    int *fw = malloc(sizeof(int) * n);
    int *bk = malloc(sizeof(int) * n);
    int i, j;
    for (i = 0; i != n; i++) {
        if (i == 0) {
            fw[0] = a[0];
            bk[n - 1] = a[n - 1];
        } else {
            fw[i] = fw[i - 1] + a[i];
            bk[n - i - 1] = bk[n - i] + a[n - i - 1];
        }
    }
    // show(a, n);
    // show(fw, n);
    // show(bk, n);
    int sum = fw[n - 1];
    int max = sum;
    for (i = 0; i != n; i++) {
        for (j = 0; j != i; j++) {
            int add = fw[i] + bk[j];
            if (add > max) {
                max = add;
            }
        }
    }
    max -= sum;
    free(fw);
    free(bk);
    return max;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        int *a = malloc(sizeof(int) * n);
        int i;
        for (i = 0; i != n; i++) {
            scanf("%d", &a[i]);
        }
        printf("%d\n", maxsum(a, n));
        free(a);
    }
    return 0;
}
