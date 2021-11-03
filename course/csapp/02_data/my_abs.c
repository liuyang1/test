/* ref: borretii.me/article/signed-integers-asymmetrical */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

int8_t my_abs(int8_t x) {
    // trying to take ABS value of most negative integer is NOT defined
    // return abs(x);
    if (x >= 0) {
        return x;
    } else {
        // assert(x == INT8_MIN); // WRONG !!!
        return -x;
    }
}

int main() {
    int8_t xs[] = {0,
                   INT8_MAX - 1,
                   INT8_MAX,
                   // INT8_MAX + 1, // overflow here first, INT8_MIN
                   INT8_MIN, // overflow when ABS
                   INT8_MIN + 1};
    size_t i;
    for (i = 0; i != sizeof(xs) / sizeof(xs[0]); i++) {
        int8_t x = xs[i];
        printf("abs(%4i)=%4i\n", x, my_abs(x));
    }
    return 0;
}
