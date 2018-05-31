/** ref: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#include <stdio.h>

unsigned int leftmost(unsigned int n) {
    return n ^ (n >> 1);
}
void test_leftmost() {
    unsigned int i;
    for (i = 1; i != 256; i++) {
        printf("leftmost(0x%02x)=0x%02x\n", i, leftmost(i));
    }
}
/** Returns one plus the index of the least significant 1-bit of x,
 * or if x is zero, returns zero.
 */
void test_ffs() {
    unsigned int i;
    for (i = 1; i != 256; i = i << 1) {
        printf("ffs(0x%02x)=%d\n", i, __builtin_ffs(i));
    }
    // for (i = 0; i != 256; i++) {
    //     printf("ffs(0x%02x)=%d\n", i, __builtin_ffs(i));
    // }
}

/** Built-in Function: int __builtin_clz (unsigned int x)
 * Returns the number of leading 0-bits in x,
 * starting at the most significant bit position.
 * If x is 0, the result is undefined.
 */
void test_clz() {
    unsigned int i;
    for (i = 1; i != 256; i = i << 1) {
        printf("clz(0x%02x)=%d\n", i, __builtin_clz(i));
    }
}

unsigned int sw_ctz(unsigned int n) {
    unsigned int cnt = 0;
    for (; n > 0; cnt++, n >>= 1) {
        ;
    }
    return cnt - 1;
}

unsigned int builtin_ctz(unsigned int n) {
    return __builtin_ctz(n);
}

/**
 * Built-in Function: int __builtin_ctz (unsigned int x)
 * Returns the number of trailing 0-bits in x,
 * starting at the least significant bit position.
 * If x is 0, the result is undefined.
 */
void test_ctz() {
    unsigned int i;
    for (i = 1; i != 256; i = i << 1) {
        printf("ctz(0x%02x)=%d\n", i, __builtin_ctz(i));
        printf("sw_ctz(0x%02x)=%d\n", i, sw_ctz(i));
    }
}

int main() {
    // test_ffs();
    test_leftmost();
    // test_clz();
    // test_ctz();
    return 0;
}
