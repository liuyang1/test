#include <stdio.h>
#define uint32_t unsigned int
// warn: must twice transform
#define BE_FOURCC(ch0, ch1, ch2, ch3)           \
     ((uint32_t)(unsigned char)(ch3)        |   \
     ((uint32_t)(unsigned char)(ch2) <<  8) |   \
     ((uint32_t)(unsigned char)(ch1) << 16) |   \
     ((uint32_t)(unsigned char)(ch0) << 24) )

static unsigned int a[] = {
    BE_FOURCC('0', '1', '2', '3'),
    BE_FOURCC('\0', '\0', '\0', 'G'),
};
int main(void)
{
    unsigned int v = BE_FOURCC('a', 'b', 'c', 'd');
    printf("0x%x\n", v);
    printf("0x%x\n", a[0]);
    return 0;
}
