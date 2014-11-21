#include <stdio.h>

#define mycall(fn, expect_ret, args...) \
    {\
        printf("call %s %s %d\n", #fn, #args, __LINE__);\
        if( expect_ret != fn(args))\
            printf("error\n");\
        printf("call "#fn" over\n");\
    }

int add(int a, int b)
{
    return a + b;
}

int main()
{
    int a = 1;
    mycall(add, 0, a, -1);
    return 0;
}
