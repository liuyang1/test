#include <stdio.h>

/*
 * `-pedantic' and other options cause warnings for many GNU C
 * extensions.  You can prevent such warnings within one expression by
 * writing `__extension__' before the expression.  `__extension__' has no
 * effect aside from this.
 */
#define FUNC(a) (__extension__ ({              \
                                int b = 2 *a ; \
                                b;             \
                                }))


int main() {
    int a = 21;
    int ret = FUNC(a);
    printf("FUNC(%d) = %d\n", a, ret);
    return 0;
}
