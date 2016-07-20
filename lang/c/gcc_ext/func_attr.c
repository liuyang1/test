#include <stdlib.h>
#include <stdio.h>

// deprecated attr
int __attribute__((deprecated)) silly_add(int x, int y)
{
    return x + y;
}
// warn_unused_result attr
void * __attribute__((warn_unused_result)) alloc(int n)
{
    return malloc(n);
}
// __const__ function
int __attribute__((__const__)) square(int x)
{
    return x * x;
}

int main()
{
    int x = 7;
    printf("x: %d x^2: %d\n", x, square(x));

    char *p = alloc(x);
    // get a warning, as not checking return value of ALLOC func
    alloc(x);
    printf("p: %p\n", p);

    // get a warning, as use deprecated func
    x = silly_add(3, 4);

    free(p);
    return 0;
}
