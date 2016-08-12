#include <string.h>
#include <stdio.h>

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

uint32_t swap_words(uint32_t arg)
{
    uint16_t *const sp = (uint16_t *)&arg;
    uint16_t        hi = sp[0];
    uint16_t        lo = sp[1];

    sp[1] = hi;
    sp[0] = lo;

    return (arg);
}

void case0()
{
    int a = 0x12345678;
    short *b = (short *)&a;
    printf("int pointer %p -> short pointer %p\n", &a, b);
    printf("it's same but break strict-aliasing rules\n");
}

void case1()
{
    uint32_t v = 0xdeadbeef;
    printf("%x -> swap_words:%x\n", v, swap_words(v));
}

int main()
{
    case0();
    case1();
    return 0;
}
