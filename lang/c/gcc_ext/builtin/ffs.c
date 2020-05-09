#include <stdio.h>
int loop_bit_low_to_high() {
    printf("%s\n", __func__);
    unsigned int x = 0xf7, last = 0;
    int bit;
    for (; x != 0; x = x & (~(1<<(bit - 1)))) {
        bit = __builtin_ffs(x);
        printf("x=0x%x bit=%d\n", x, bit);
        if (last == x) {
            break;
        }
        last = x;
    }

    return 0;
}

int loop_bit_high_to_low() {
    printf("%s\n", __func__);
    unsigned int x = 0xf7, last = 0;
    int bit;
    for (; x != 0; x = x & (~(1<<(bit - 1)))) {
        bit = sizeof(unsigned int) * 8 - __builtin_clz(x);
        printf("x=0x%x bit=%d\n", x, bit);
        if (last == x) {
            break;
        }
        last = x;
    }

    return 0;
}

int main() {
    loop_bit_low_to_high();
    loop_bit_high_to_low();
    return 0;
}
