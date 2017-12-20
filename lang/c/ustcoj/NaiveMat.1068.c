#include <stdio.h>
#include <stdlib.h>

void showArr(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
int naiveMat(int b1, int q, int a1, int d, int x, int y) {
    int *a = malloc(sizeof(int) * (x + 1));
    int i, j;
    a[0] = 0;
    a[1] = a1;
    for (i = 2; i != x + 1; i++) {
        a[i] = a[i - 1] + d;
    }
    int an = b1;
    for (i = 1; i != y + 1; i++) {
        a[0] = an;
        for (j = 1; j != x + 1; j++) {
            a[j] += a[j - 1];
        }
        an *= q;
    }
    int r = a[x];
    free(a);
    return r;
}

int main() {
    int i, n;
    scanf("%d", &n);
    for (i = 0; i != n; i++) {
        int b1, q, a1, d, x, y;
        scanf("%d%d%d%d%d%d", &b1, &q, &a1, &d, &x, &y);
        printf("Case #%d: %d\n", i + 1, naiveMat(b1, q, a1, d, x, y));
    }
    return 0;
}
