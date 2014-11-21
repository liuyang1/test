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

    printf("Actually only GCC-EXTEND style and STD style are struct INIT.\n");
    return 0;
}
