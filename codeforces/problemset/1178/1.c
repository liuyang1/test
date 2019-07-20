#include <stdio.h>
#include <stdlib.h>

unsigned sum(unsigned int *a, unsigned int n) {
    unsigned int i, s;
    for (i = s = 0; i != n; i++) {
        s += a[i];
    }
    return s;
}

void sln(unsigned int *a, unsigned int n) {
    unsigned int s = sum(a, n);
    if (a[0] * 2 > s) {
        printf("1\n1\n");
        return;
    }
    unsigned int i;
    for (i = 1; i != n; i++) {
        if (a[i] * 2 > s) {
            printf("0\n");
            return;
        }
    }
    unsigned int s1, cnt;
    for (i = 1, s1 = a[0], cnt = 1; i != n; i++) {
        if (a[i] * 2 <= a[0]) {
            s1 += a[i];
            cnt++;

        }
    }
    if (s1 * 2 > s) {
        printf("%d\n1", cnt);
        for (i = 1; i != n; i++) {
            if (a[i] * 2 <= a[0]) {
                printf(" %d", i + 1);
            }
        }
        printf("\n");
        return;
    }
    printf("0\n");
}

int main() {
    unsigned int n;
    scanf("%u", &n);
    unsigned int *a = malloc(sizeof(unsigned int) * n);
    unsigned int i;
    for (i = 0; i != n; i++) {
        scanf("%u", &a[i]);
    }

    sln(a, n);

    free(a);
    return 0;
}
