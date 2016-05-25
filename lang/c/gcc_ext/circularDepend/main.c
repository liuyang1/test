#include <stdio.h>
#include "base.h"

int main() {
    int x = func_depend_b(2);
    int y = func_depend_a(2);
    printf("%d\n", x);
    printf("%d\n", y);
    return 0;
}
