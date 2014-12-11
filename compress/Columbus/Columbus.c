#include <stdio.h>

#define BIT_SET(a, b)   ((a) |= (1 << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1 << (b)))
#define BIT_GET(a, b)   (((a) >> (b)) & 1)

#define BIT_SETV(a, b, v) {if (v > 0) \
                               BIT_SET(a, b); \
                           else    \
                               BIT_CLEAR(a, b); }

int readBit(char* data, int bitoffset)
{
    data += bitoffset / 8;
    bitoffset %= 8;
    bitoffset = 7 - bitoffset;
    return BIT_GET(*data, bitoffset);
}

int ColumbusDecode(char* d)
{
    int bitoffset = 0;
    int leadingZeroBits = -1;
    int b;
    for (b = 0; !b; leadingZeroBits++) {
        b = readBit(d, bitoffset++);
    }
    int base = (1 << leadingZeroBits) - 1;
    int code = 0, v;
    for (int i = 0; i < leadingZeroBits; i++) {
        code <<= 1;
        v = readBit(d, bitoffset++);
        BIT_SETV(code, 0, v);
    }
    code += base;
    return code;
}

void testcase()
{
    char data[] = {
        0b10000000, 0b01000000, 0b01100000, 0b00100000, 0b00101000,
        0b00110000, 0b00111000, 0b00010000, 0b00010010, 0b00010100,
        0b00010110,
    };
    int v;
    for (int i = 0; i < sizeof(data); i++) {
        v = ColumbusDecode(data + i);
        if (i != v) {
            printf("error when decode data 0x%02x should be %d actually %d\n", data[i], i, v);
            return;
        }
    }
    printf("test case passed\n");
}

int main()
{
    testcase();
    return 0;
}
