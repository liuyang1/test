#include <stdio.h>

int test_func1()
{
    printf("test_func0 level: %d addr: %p\n",
           0, __builtin_return_address(0));
    printf("main       level: %d addr: %p\n",
           1, __builtin_return_address(1));
    return 0;
}
int test_func0()
{
    return test_func1();
}
int main()
{
    test_func0();
    return 0;
}
