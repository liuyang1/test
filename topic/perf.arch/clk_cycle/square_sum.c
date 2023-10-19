#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define PRIME 2147483629

int main() {
    uint64_t i, s;
    for (i = s = 0; i != 1024ll * 1024ll * 1024ll; i++) {
        s += (i * i) % PRIME;
    }
    printf("%"PRIu64"\n", s);
    return 0;
}

