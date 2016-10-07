#include <stdio.h>
#include <limits.h>

#define SHOW(fmt, a) printf("%-20s: " fmt "\n", # a, a);

int main() {
    SHOW("%d", CHAR_BIT);
    SHOW("%d", SCHAR_MIN);
    SHOW("%d", SCHAR_MAX);
    SHOW("%d", UCHAR_MAX);
    SHOW("%d", CHAR_MIN);
    SHOW("%d", CHAR_MAX);

    // max number of bytes in a multibyte char for any locale
    SHOW("%d", MB_LEN_MAX);

    SHOW("%hd", SHRT_MIN);
    SHOW("%hd", SHRT_MAX);
    SHOW("%hu", USHRT_MAX);

    printf("%-20s: 01234567890123456789\n", "ruler");

    SHOW("%d", INT_MIN);
    SHOW("%d", INT_MAX);
    SHOW("%u", UINT_MAX);

    SHOW("%ld", LONG_MIN);
    SHOW("%ld", LONG_MAX);
    SHOW("%lu", ULONG_MAX);

    SHOW("%lld", LLONG_MIN);
    SHOW("%lld", LLONG_MAX);
    SHOW("%llu", ULLONG_MAX);
}
