typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

// optimize: check table
uint8_t reverseBytes(uint8_t n)
{
    uint8_t ret = 0;
    int i;
    for (i = 0; i != 8; i++) {
        if (n & 0x1) {
            ret |= (1 << (7 - i));
        }
        n >>= 1;
    }
    return ret;
}
// optimize: big/little endian
uint32_t reverseBits(uint32_t n) {
    uint32_t ret = 0;
    int i;
    for (i = 0; i != 4; i++) {
        ret = (ret << 8) | reverseBytes(n % 256);
        n /= 256;
    }
    return ret;
}

#include <stdio.h>
int main()
{
    unsigned char c = 0x23;
    printf("%x %x\n", c, reverseBytes(c));
    unsigned int val = 0xdeadbeef;
    printf("%x %x\n", val, reverseBits(val));
    val = 1;
    printf("%x %x\n", val, reverseBits(val));
}
