#include "stdio.h"

int main(void)
{
    const char *style;

    style = "index style";
    int a0[] = {[0] = 1, 2, [2]= 3};
    // each initializer element taht does not have a designator applies to the
    // next consecutive element of the arrya or struct.
    // So 2 init to index 1 as it after index 0
    // ref:[[https://gcc.gnu.org/onlinedocs/gcc-4.4.1/gcc/Designated-Inits.html#Designated-Inits]]
    printf("%s: %d %d %d\n", style, a0[0], a0[1], a0[2]);

    style = "range style, [first ... last] = value";
    int a1[] = {[0 ... 1] = 1, [2] = 3};
    printf("%s: %d %d %d\n", style, a1[0], a1[1], a1[2]);

    // index also can use charcters or `enum` type.

    return 0;
}
