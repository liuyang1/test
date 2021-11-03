#include <stdio.h>

/**
 * XOR means true iff x != y (at bits level)
 * so we negative the result of XOR
 * y\x 0 1
 * 0   1 0 = ! 0 1
 * 1   0 1     1 0
 */
int my_equal(int x, int y) {
    return !(x ^ y);
}

int ref_equal(int x, int y) {
    return x == y;
}

int unit(int x, int y) {
    int a = my_equal(x, y);
    int b = ref_equal(x, y);
    if (a != b) {
        printf("equal(%x, %x) = %x != %x\n", x, y, a, b);
        return 0;
    }
    return 1;
}

int main() {
    unit(2, 3);
    unit(3, 2);
    unit(2, 2);
    unit(3, 3);
    return 0;
}
