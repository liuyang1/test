#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int i, int j) {
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void lshift(int *a, int n, int c) {
    int t = a[c], i;
    for (i = c; i != n - 1; i++) {
        a[i] = a[i + 1];
    }
    a[n - 1] = t;
}

void show(int *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%d", a[i]);
    }
    printf("\n");

}

void help(int *a, int n, int c) {
    if (c == n - 1) {
        show(a, n);
        return;
    }
    int i;
    for (i = c; i != n; i++) {
        help(a, n, c + 1);
        if (i == n - 1) {
            break;
        }
        swap(a, c, i + 1);
    }
    lshift(a, n, c);
}

void permu(int n) {
    int *a = malloc(sizeof(int) * n), i;
    for (i = 0; i != n; i++) {
        a[i] = i + 1;
    }
    help(a, n, 0);
    free(a);
}

int main() {
    int n, m;
    scanf("%d", &n);
    permu(n);
    return 0;
}
