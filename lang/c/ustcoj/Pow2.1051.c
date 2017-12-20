#include <stdio.h>

int pow2(int n) {
    return 1 << n;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        printf("%d\n", pow2(n));
    }
    return 0;
}
