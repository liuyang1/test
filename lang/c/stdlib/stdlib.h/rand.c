#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // In the GNU C Library, it is 2147483647, which is the largest signed integer representable in
    // 32 bits. In other libraries, it may be as low as 32767.
    printf("%s: %d 0x%08X\n", "RAND_MAX", RAND_MAX, RAND_MAX);
    printf("RAND_MAX=%d ?= INT_MAX=%d %s\n",
           RAND_MAX, INT_MAX,
           RAND_MAX == INT_MAX ? "true" : "false");

	// rand is SAME sequence, without seed of srand
    int i;
    for (i = 0; i != 10; i++) {
        printf("i=%d %d\n", i, rand());
    }
    return 0;
}
