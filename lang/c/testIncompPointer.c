#include <stdio.h>
void func(void *ptr)
{
    *ptr = 1;
}
void func2(void **ptr)
{
    *ptr = 3;
}

int main(void)
{
    int *ptr = 2;
    void *t;
    printf("ptr: %p\n", ptr);
    func(&t);
    ptr = t;
    printf("ptr: %p\n", ptr);
    return 0;
}
