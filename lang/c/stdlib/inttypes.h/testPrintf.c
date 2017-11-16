#include <stdio.h>
#include <inttypes.h>

int main() {
    uint32_t a = 0;
    printf("a=%d\n", a);
    puts("a=%" PRIu32 "\n");
    printf("a=%" PRIu32 "\n", a);
    return 0;
}
