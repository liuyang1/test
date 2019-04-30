/** ref: https://gcc.gnu.org/onlinedocs/gcc-3.0.2/cpp_4.html#SEC39
 */

#include <stdio.h>

int main() {
#define A
#define B
#if defined A && defined B
    printf("defined A and defined B\n");
#endif

#undef A
#undef B
#if defined(A) || defined(B)
    printf("defined A or defined B\n");
#endif
    return 0;
}
