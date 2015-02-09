#include <stdio.h>
int main(void)
{
    char a = 200;
    printf("%d %c\n", a, a);
    unsigned char b = 200;
    printf("%u %c\n", b, b);
    printf("\nsigned bit, will all 1. so head byte of a, is 0xffffffc8\n");
    int x = a << 8 | a;
    printf("%#x %d\n", x, x);
    unsigned int y = b << 8 | b;
    printf("%#x %d\n", y, y);
    return 0;
}
