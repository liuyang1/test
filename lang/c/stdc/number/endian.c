#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t a = 0x0A0B0C0D;
    printf("origin data: %#010x\n", a);
    printf("cast:        %#02x\n", (int8_t)a);
    int8_t *c = (int8_t *)&a;
    printf("byte view:   %02x %02x %02x %02x\n", 0, 1, 2, 3);
    printf("             %02x %02x %02x %02x\n", c[0], c[1], c[2], c[3]);
    return 0;
}
