/** fib */
/** 0: too easy */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

int main() {
    int t, n, i;
    uint64_t a, b;
    for (scanf("%d", &t); t != 0; t--) {
        for (scanf("%d", &n), i = a = 0, b = 1; i != n; i++) {
            // printf("i=%d %" PRIu64 "\n", i, b);
            printf("%"PRIu64" ", b);
            b = b + a;
            assert(b > a);
            a = b - a;
        }
        printf("\n");
    }
    return 0;
}
