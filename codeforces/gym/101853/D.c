#include <stdio.h>
#include <stdlib.h>

int countPositive(int *a, int n) {
    int i, m;
    for (i = 0, m = 0; i != n; i++) {
        m += a[i] != 0;
    }
    return m;
}

int main() {
    int T;
    scanf("%d", &T);
    for (; T != 0; T--) {
        int n;
        scanf("%d", &n);
        int *a = malloc(sizeof(int) * n), i;
        for (i = 0; i != n; i++) {
            scanf("%d", a + i);
        }
        int m = countPositive(a, n);
        printf("%d\n", m);
        free(a);
    }
}
