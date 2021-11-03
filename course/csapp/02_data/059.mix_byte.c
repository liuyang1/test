#include <stdio.h>
#include <assert.h>

// P2.59
int mix_byte(int x, int y) {
    char *px = (char *)&x;
    char *py = (char *)&y;
    *py = *px; // move LSB from x to y
    return y;
}

// P2.60
int replace_byte(unsigned x, int i, unsigned char b) {
#define WORD_NUM        4
    assert(i < WORD_NUM);
    char *px = (char *) &x;
    px[i] = b;
    return x;
}

int main() {
    int x = 0x89ABCDEF;
    int y = 0x76543210;
    int z = mix_byte(x, y);
    printf("%X\n", z);

    printf("%X\n", replace_byte(0x12345678, 2, 0xAB)); // 0x12AB5678
    printf("%X\n", replace_byte(0x12345678, 0, 0xAB)); // 0x123456AB
    return 0;
}
