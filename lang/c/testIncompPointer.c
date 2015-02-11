#include <stdio.h>
#define intptr_t long
void func2(void **ptr)
{
    *ptr = (char *)3;
}

int main(void)
{
    void *ptr = (int *)-1;
    int *intp = (int *)-2;
    printf("ptr: %p\n", ptr);
    func2(&ptr);
    intp = ptr;
    printf("ptr: %p\n", ptr);
    printf("intp: %p\n", intp);
    func2(&intp);
    return 0;
}
