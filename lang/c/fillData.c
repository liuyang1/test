#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

UINT32 maskTbl[] = {
    (0x1 << 0) - 1,
    (0x1 << 1) - 1,
    (0x1 << 2) - 1,
    (0x1 << 3) - 1,
    (0x1 << 4) - 1,
    (0x1 << 5) - 1,
    (0x1 << 6) - 1,
    (0x1 << 7) - 1,
    (0x1 << 8) - 1,
};
/*
UINT32 maskTbl[] = {
    (0x1 << 8) - 1,
    (0x1 << 7) - 1,
    (0x1 << 6) - 1,
    (0x1 << 5) - 1,
    (0x1 << 4) - 1,
    (0x1 << 3) - 1,
    (0x1 << 2) - 1,
    (0x1 << 1) - 1,
    (0x1 << 0) - 1,
};
*/
// fill data pointer P, start OFFSET, fill LEN size data from V, (start low-bit)
inline static void fillData(UINT8 *p, UINT32 offset, UINT32 len, UINT64 v)
{
    UINT32 byteoffset = offset / 8;
    UINT32 bitoffset = offset % 8;
    p += byteoffset;
    UINT64 vv;
    if (bitoffset != 0) {
        // extract original bitoffset data;
        vv = (*p) & maskTbl[bitoffset];
        // merge original data and filling data
        vv = vv | (v << bitoffset);
    } else {
        vv = v;
    }
    if (bitoffset + len > 64) {
        printf("fillData overflow\n");
    }
    UINT32 i;
    for (i = 0; i < (bitoffset + len) / 8; i++) {
        p[i] = (vv >> i * 8) & 0xff;
    }
    bitoffset = (bitoffset + len) % 8;
    if (bitoffset != 0) {
        p[i] = (vv >> i * 8) & maskTbl[bitoffset];
    }
}

inline static void dispBuf(UINT8 *p, UINT32 len)
{
    UINT32 i;
    for (i = 0; i < len; i++) {
        printf("%02x ", p[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 4 == 0) {
            printf("\t");
        }
    }
    if (i % 16 != 0) {
        printf("\n");
    }
}
inline static void fillDataTest()
{
    printf("fillData test begin\n");
    UINT8 buf[16];
    memset(buf, 0, 16);
    fillData(buf, 0, 3, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 0, 10, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 0, 15, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 0, 20, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 5, 10, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 10, 10, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 15, 10, -1);
    dispBuf(buf, 16);
    printf("fillData test end\n");
}

static int saveBuf(const char *filename, void *p, size_t size)
{
    FILE* fp = fopen(filename, "wb");
    UINT32 ret = fwrite(p, size, 1, fp);
    if (ret != 1) {
        ret = ferror(fp);
        printf("write data error: %d\n", ret);
    } else {
        ret = 0;
    }
    fclose(fp);
    return ret;
}

int main(void)
{
    fillDataTest();
    return 0;
}
