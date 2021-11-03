#include <stdio.h>
// set, if mask, then 1
// m\x 0 1
// 0   0 1
// 1   1 1
int bis(int x, int m) {
    return x | m;
}

// clear, if mask, then 0
// m\x 0 1
// 0   0 1
// 1   0 0
int bic(int x, int m) {
    return x & (~m);
}

#define CHECK_EQ(a, b) {                                  \
        int _a = a, _b = b;                               \
        printf("%s=%#x ?= %s=%#x %s\n", # a, _a, # b, _b, \
               (_a == _b) ? "SUCC" : "FAIL");             \
}

int test_bi() {
    CHECK_EQ(bis(0x0f, 0x33), 0x3f);
    CHECK_EQ(bic(0x0f, 0x33), 0x0c);
    return 0;
}

// y\x 0 1
// 0   0 1
// 1   1 1
int bool_or(int x, int y) {
    return bis(x, y);
}

// y\x 0 1
// 0   0 0
// 1   0 1
int bool_and(int x, int y) {
    return bic(x, bic(0xFFFFFFFF, y));
}

// y\x 0 1
// 0   0 1  = 0 1     0 0
// 1   1 0    0 0 OR  1 0
int bool_xor(int x, int y) {
    bool_or(bic(x, y), bic(y, x));
}

int test_bool() {
    CHECK_EQ(bool_or(0x0f, 0x33), 0x3f);
    CHECK_EQ(bool_and(0x0f, 0x33), 0x03);
    CHECK_EQ(bool_xor(0x0f, 0x33), 0x3c);
    return 0;
}

int main() {
    test_bi();
    test_bool();
    return 0;
}
