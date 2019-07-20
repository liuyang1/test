#include <stdio.h>

int isPrime(int x) {
    int f;
    for (f = 2; f * f <= x; f++) {
        if (x % f == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int x;
    for (x = 200 * 1000; ; x++) {
        if (isPrime(x)) {
            printf("%d\n", x);
            break;
        }
    }
    return 0;
}
