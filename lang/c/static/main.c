#include <stdio.h>
int exFunc();

static int func()
{
    printf("func in main\n");
    return 0;
}
int main(void)
{
    func();
    exFunc();
    return 0;
}

