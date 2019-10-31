/**
 * Based on float IEEE.754 representation
 *
 * float is 32bits
 * - 1bit: sign
 * - 8bit: exponent
 * - 23bit: fraction
 *
 * 0.15625 = (1+0.25) * 2^(-3)
 *         = (-1)^0 * (1+0.25) * 2^(124-127)
 *         = (-1)^sign * (1+fraction) * 2^(e-127)
 * - sign=0
 * - exponent=124=0b01111100
 * - fraction=0.25=0b01000,,,
 *
 * so it's binary representation is 0 0111_1100 010000
 * 0x3e200000
 */
#include <stdio.h>
typedef union {
    double f;
    long long int x;
} T1;

typedef union {
    float f;
    int x;
} T2;

int main() {
    T1 t;
    t.f = 0.15625;
    printf("%llx %lf %zu\n", t.x, t.f, sizeof(t));

    T2 t2;
    t2.f = 0.15625;
    printf("%x %f %zu\n", t2.x, t2.f, sizeof(t2));

    /** !!!
     * directly
     * printf("%x\n", 0.15625);
     * This is wrong, printf function will do more convertion
     */

    return 0;
}
