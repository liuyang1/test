#include <stdio.h>
#define uint32_t unsigned int
// warn: must twice transform
#define BE_FOURCC(ch0, ch1, ch2, ch3)           \
     ((uint32_t)(unsigned char)(ch3)        |   \
     ((uint32_t)(unsigned char)(ch2) <<  8) |   \
     ((uint32_t)(unsigned char)(ch1) << 16) |   \
     ((uint32_t)(unsigned char)(ch0) << 24) )
#define ERR_FOURCC(ch0, ch1, ch2, ch3) \
     ((ch3)        |   \
     ((ch2) <<  8) |   \
     ((ch1) << 16) |   \
     ((ch0) << 24) )

#define VERIFY(ch0, ch1, ch2, ch3) \
{ \
    ncase++; \
    unsigned int a = BE_FOURCC(ch0, ch1, ch2, ch3); \
    unsigned int b = ERR_FOURCC(ch0, ch1, ch2, ch3); \
    if (a != b) { \
        printf("%d case:\t%#x != %#x\n", ncase, a, b); \
    } \
}

int main(void)
{
    char c = 0x80;
    unsigned int v = ERR_FOURCC('a', 'b', c, 'd');
    printf("0x%x\n", v);

    unsigned int ncase = 0;
    VERIFY('0', '1', '2', '3');
    VERIFY('\0', '\0', '\0', 'G');
    VERIFY(129, 'b', 'c', 'd');
    VERIFY('a', 'b', 255, 'd');
    VERIFY('a', 'b', (char)0x80, 'd');
    // VERIFY('a', 'b', 'ÿ', 'd');
    // VERIFY('a', 'b', '¶', 'd');
    VERIFY('a', 'b', (1<<16) - 1, 'd');
    return 0;
}
