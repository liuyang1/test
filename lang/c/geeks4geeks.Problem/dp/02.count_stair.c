/** count stair, with step 1, or 2 */
/** acutally, this is fib again */
/** 0: easy */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

int main() {
    uint64_t r = 1000 * 1000 * 1000 + 7;
    int t, n, i;
    uint64_t a, b;
    for (scanf("%d", &t); t != 0; t--) {
        for (scanf("%d", &n), i = a = 0, b = 1; i != n; i++) {
            int t = b;
            b = (b + a) % r;
            a = t; // The minus may be underflow
        }
        printf("%" PRIu64 "\n", b);
    }
    return 0;
}
