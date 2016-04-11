// show CHAR, SHORT, INT, LONG, LONG LONG, SIGNED, UNSIGNED, FLOAT, DOUBLE
// type range with standard libarary or self direct computation.
#include <limits.h>
#include <float.h>
#include <stdio.h>

#define TYPESTRFMT "%-20s"
void show_std(void) {
    printf(TYPESTRFMT " type range [%d, %d]\n", "char", CHAR_MIN, CHAR_MAX);
    printf(TYPESTRFMT " type range [%d, %d]\n", "short", SHRT_MIN, SHRT_MAX);
    printf(TYPESTRFMT " type range [%d, %d]\n", "int", INT_MIN, INT_MAX);
    printf(TYPESTRFMT " type range [%ld, %ld]\n", "long", LONG_MIN, LONG_MAX);
    printf(TYPESTRFMT " type range [%ld, %ld]\n", "long long", LONG_MIN, LONG_MAX);

    printf(TYPESTRFMT " type range [%u, %u]\n", "unsigned char", 0, UCHAR_MAX);
    printf(TYPESTRFMT " type range [%u, %u]\n", "unsigned short", 0, USHRT_MAX);
    printf(TYPESTRFMT " type range [%u, %u]\n", "unsigned int", 0, UINT_MAX);
    printf(TYPESTRFMT " type range [%lu, %lu]\n", "unsigned long", 0l, ULONG_MAX);

    printf(TYPESTRFMT " type range [%f, %f]\n", "float", FLT_MIN, FLT_MAX);
    printf(TYPESTRFMT " type range [%f, %f]\n", "double", DBL_MIN, DBL_MAX);
}

void show_self(void) {
    printf(TYPESTRFMT " type range [%d, %d]\n", "char", (signed char)0x80, 0x7f);
    printf(TYPESTRFMT " type range [%d, %d]\n", "short", (signed short)(0x1 << 15), 0x7fff);
    printf(TYPESTRFMT " type range [%d, %d]\n", "int", (1 << 31), 0x7fffffff);
    printf(TYPESTRFMT " type range [%ld, %ld]\n", "long", (1l << 63), (0x1l << 64) - 1);
    printf(TYPESTRFMT " type range [%ld, %ld]\n", "long long", LONG_MIN, LONG_MAX);

    printf(TYPESTRFMT " type range [%u, %u]\n", "unsigned char", 0, UCHAR_MAX);
    printf(TYPESTRFMT " type range [%u, %u]\n", "unsigned short", 0, USHRT_MAX);
    printf(TYPESTRFMT " type range [%u, %u]\n", "unsigned int", 0, UINT_MAX);
    printf(TYPESTRFMT " type range [%lu, %lu]\n", "unsigned long", 0l, ULONG_MAX);

    printf(TYPESTRFMT " type range [%f, %f]\n", "float", FLT_MIN, FLT_MAX);
    printf(TYPESTRFMT " type range [%f, %f]\n", "double", DBL_MIN, DBL_MAX);
}

int main(void) {
    show_std();
    show_self();
    return 0;
}
