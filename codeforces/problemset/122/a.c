#include <stdio.h>
#include <stdlib.h>

int isLucky(unsigned int x) {
    if (x == 0) {
        return 0;
    }
    for (; x != 0; x /= 10) {
        unsigned int d = x % 10;
        if (d != 4 && d != 7) {
            return 0; // no lucky
        }
    }
    return 1; // lucky
}

int isAlmostLucky(unsigned int x) {
    unsigned int f;
    for (f = 2; f * f <= x; f++) {
        if (x % f == 0 && (isLucky(f) || isLucky(x / f))) {
            return 1;
        }
    }
    return isLucky(x); // prime
}

int main() {
    unsigned int x;
    scanf("%u", &x);
    printf("%s\n", isAlmostLucky(x) ? "YES" : "NO");
    return 0;
}
