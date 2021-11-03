#include <stdio.h>
#include <stdlib.h>

int tadd_ok(int x, int y) {
    if (x > 0) {
        if (y > 0) {
            return x + y > 0;
        }
    } else {
        if (y < 0) {
            return x + y < 0;
        }
    }
    return 1;
}

// Practice 2.31 wrong tadd_ok
int wrong_tadd_ok(int x, int y) {
    /**
     * it's overflow, too when do minus
     */
    int s = x + y;
    return (s - x == y) && (s - y == x);
}

int check_add(int x, int y) {
    int b = tadd_ok(x, y);
    int wrong = wrong_tadd_ok(x, y);
    printf("x=%d + y=%d = %d %s %s\n",
           x, y, x + y,
           b ? "safe" : "OVERFLOW",
           wrong ? "safe" : "OVERFLOW");
    return b;
}

int main() {
    check_add(0, 0);
    int max = 0x7fffffff;
    int min = (-max - 1); // C std define this way
    check_add(max, max);
    check_add(max, min);
    check_add(min, max);
    check_add(min, min);
    return 0;
}
