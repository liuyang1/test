#include <assert.h>
#include <stdio.h>
#include <limits.h>

int main(void)
{
    unsigned int a = -1;
    if (a > INT_MAX) {
        printf("a %u greater than %d\n", a, INT_MAX);
    }
    assert(a > INT_MAX);
    printf("assert a greater that INT_MAX, OK\n");
    printf("assert a less that INT_MAX, ABORT!\nVVV\n");
    assert(a < INT_MAX);
    return 0;
}
