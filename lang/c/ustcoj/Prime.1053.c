#include <stdio.h>
#include <stdlib.h>

void showPrime(int n) {
    int *a = malloc(sizeof(int) * n);
    int i, j;
    for (i = 0; i != n; i++) {
        a[i] = 1;
    }
    for (i = 2; i != n; i++) {
        if (a[i] == 0) {
            continue;
        }
        for (j = i * 2; j < n; j += i) {
            a[j] = 0;
        }
    }
    char *lead = "";
    for (i = 2; i != n; i++) {
        if (a[i]) {
            printf("%s%d", lead, i);
            lead = ", ";
        }
    }
    printf("\n");
}
int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        showPrime(n + 1);
    }
    return 0;
}
