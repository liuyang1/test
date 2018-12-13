#include <stdio.h>
#include <stdint.h>

#define SHOW(fmt, a) printf("%-20s: " fmt "\n", # a, a);

int main() {
    SHOW("%d", UINT8_MAX);
    SHOW("%d", INT8_MIN);
    SHOW("%d", INT8_MAX);

    SHOW("%hd", INT16_MIN);
    SHOW("%hd", INT16_MAX);
    SHOW("%hu", UINT16_MAX);

    printf("%-20s: 01234567890123456789\n", "ruler");

    SHOW("%d", INT32_MIN);
    SHOW("%d", INT32_MAX);
    SHOW("%u", UINT32_MAX);

    SHOW("%ld", INT64_MIN);
    SHOW("%ld", INT64_MAX);
    SHOW("%lu", UINT64_MAX);
    return 0;
}
