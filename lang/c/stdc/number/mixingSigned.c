#include <stdint.h>
#include <stdio.h>

int main() {
    int16_t a = 32760;
    int16_t b = 32754;
    printf("%hd\n", b - a);
    uint32_t num = 1, den = 3;
    // XXX: mixing signed number and unsigned number,
    // it will convert to signed number.
    int16_t t = (b - a) * num / den;
    printf("%hd\n", t);
    int16_t r = a + t;
    printf("%hd\n", r);

    return 0;
}
