#include "stdio.h"
#define COLOR_TABLE \
    X(red, "red") \
    X(green, "green") \
    X(blue, "blue") \

#define X(a, b) a,
enum Color {
    COLOR_TABLE
};
#undef X

#define X(a, b) b,
char *colorname[] = {
    COLOR_TABLE
};
#undef X

int main(void)
{
    enum Color c = red;
    printf("c=%s\n", colorname[c]);
    return 0;
}
