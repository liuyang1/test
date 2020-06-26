/** square in retangle */
/** 0: easy */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint64_t square(int m, int n) {
    uint64_t c;
    for (c = 0; m > 0 && n > 0; m--, n--) {
        c += m * n;
    }
    return c;
}

int main() {
    int t, m, n;
    for (scanf("%d", &t); t != 0; t--) {
        scanf("%d%d", &m, &n);
        printf("%" PRIu64 "\n", square(m, n));
    }
    return 0;
}
