#include <stdio.h>
#include <stdlib.h>

int main() {
    // In the GNU C Library, it is 2147483647, which is the largest signed integer representable in
    // 32 bits. In other libraries, it may be as low as 32767.
    printf("%s: %d\n", "RAND_MAX", RAND_MAX);
    return 0;
}
