#include <stdint.h>
#include <stdio.h>

int main() {
    uint32_t T;
    scanf("%u", &T);
    while (T--) {
        uint32_t n, x;
        scanf("%u%u", &n, &x);
        printf("%u\n", 2 * x);
    }
    return 0;
}
