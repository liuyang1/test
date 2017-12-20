#include <stdio.h>

int accum(int n) {
    return n * (n + 1) / 2;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        printf("%d\n", accum(n));
    }
    return 0;
}
