#include "stdio.h"
#include "string.h"
struct st {
    int a;
    int b;
};

int main(void)
{
    const char* style;
    style = "gcc extend style";
    struct st v0 = {.a = 1, .b = 2};
    printf("%s: %d %d\n", style, v0.a, v0.b);

    style = "std style";
    struct st v1 = {1, 2};
    printf("%s: %d %d\n", style, v1.a, v1.b);

    style = "stdlib memset or bzero";
    struct st v2;
    memset(&v2, 0xaa, sizeof(struct st));
    printf("%s: %d(%x) %d(%x)\n", style, v2.a, v2.a, v2.b, v2.b);

    // obsolete since GCC 2.5
    // This is GNU old-style field designator extension, NOT use it.
    // This designator style, also can EMBEDED LEVEL.
    // ref:[[https://gcc.gnu.org/onlinedocs/gcc-4.4.1/gcc/Designated-Inits.html#Designated-Inits]]
    style = "field name style";
    struct st v3 = {b: 2, a:1};
    printf("%s: %d %d\n", style, v3.a, v3.b);

    printf("Actually only GCC-EXTEND style and STD style are struct INIT.\n");
    return 0;
}
