#include <stdio.h>

int main(void)
{
    void *a = (void *)0xdeadbeef;
    void *b = (void *)0xdeadbee0;

    // not warning in gcc 4.6
    printf("a=%p b=%p diff=0x%lx\n", a, b, a - b);

    if (b > a) {
    }
    printf("comparasion on void pointer\n");
    return 0;
}
