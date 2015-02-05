#include <stdio.h>
// static func
// only work in local file
// and not affect in different file
static int func()
{
    printf("func in func1\n");
    return 0;
}

int exFunc()
{
    func();
    return 0;
}
