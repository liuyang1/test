#include "leet.h"

int getSum(int a, int b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    int c = a ^ b;
    int d = a & b;
    return getSum(c, d << 1);
}

bool unit(int a, int b) {
    int c = getSum(a, b);
    bool r = c == a + b;
    printf("getSum(%d, %d) = %d ?= %d %s\n", a, b, c, a + b, expect(r));
    return r;
}

int main() {
    unit(3, 5);
    unit(100, 97);
    return 0;
}
