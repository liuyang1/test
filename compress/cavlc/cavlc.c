#include <stdio.h>
#include "tbl.h"

#define BIT_SET(a, b)    ((a) |= (1 << (b)))
#define BIT_CLEAR(a, b)  ((a) &= ~(1 << (b)))

typedef int ErrorCode;

int loopOnData(char* in, int len, int (* func)(char))
{
    int s = 0;
    for (int i = 0; i < len; i++, in++) {
        s += func(*in);
    }
    return s;
}

int NoZero(char c)
{
    return c != 0;
}

int totalCoeffs(char* in, int len)
{
    return loopOnData(in, len, NoZero);
}

int trailingsOnes(char* in, int len)
{
    int s = 0;
    in += len - 1;
    for (int i = 0; i < len; i++, in--) {
        if (*in == 0)
            s++;
        if (s == 3 || *in != 0)
            return s;
    }
    return s;
}
int totalZeros(char* in, int len)
{
    int s = 0;
    for (int i = 0; i < len; i++, in++) {
        if (*in == 0)
            s++;
    }
    in--;
    for (int i = 0; i < len; i++, in--) {
        if (*in == 0)
            s--;
        else
            break;
    }
    return s;
}
void writeData(char* data, char c, int n)
{
    for (int i = 0; i < n; i++, data++) {
        *data = c;
    }
}

void writeBit(char* data, int bitoffset, int v)
{
    data += bitoffset / 8;
    bitoffset %= 8;
    bitoffset = 7 - bitoffset;
    if (v > 0)
        BIT_SET(*data, bitoffset);
    else
        BIT_CLEAR(*data, bitoffset);
}
ErrorCode cavlc(char* in, int insize, char* out, int outsize)
{
    int totalc = 0, trail = 0, totalz = 0;
    totalc = totalCoeffs(in, insize);
    trail = trailingsOnes(in, insize);
    char buf[256];
    int bitoffset = writeCoeffToken(buf, 0, trail, totalc);
    char* p;
    // search trailing ones flag
    int i, cnt;
    for (p = in + insize - 1, cnt = 0, i = 0; cnt < trail; i++, p--) {
        switch ((int)*p) {
        case 1:
            cnt++;
            writeBit(buf, bitoffset, 0);
            bitoffset++;
            break;
        case -1:
            cnt++;
            writeBit(buf, bitoffset, 1);
            bitoffset++;
            break;
        default:
            break;
        }
    }
    // levels encoding
    for (; i < insize; i++, p--) {
        if (*p == 0) {} else  {}
    }
    printf("bitoffset: %d\n", bitoffset);
    for (int i = 0; i < bitoffset / 8 + 1; i++) {
        printf("0x%02x\n", buf[i]);
    }
    totalz = totalZeros(in, insize);
    return 0;
}

void test()
{
    char a[] = {0, 3, 0, 1, -1,
                -1, 0, 1, 0, 0,
                0, 0, 0, 0, 0,
                0};
    int len = sizeof(a) / sizeof(a[0]);
    char buf[256];
    cavlc(a, len, buf, 256);
}

int main()
{
    test();
    return 0;
}
