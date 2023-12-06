#include <stdint.h>
#include <stdio.h>

static inline int32_t CLIP32(int64_t r) {
    // return (r > 0x7FFFFFFFLL) ? 0x7FFFFFFFLL : (r < -0x80000000LL) ? 0x80000000LL : r;
    return (r > INT32_MAX) ? INT32_MAX :
        (r < INT32_MIN) ? INT32_MIN : r;
}
int debug(int64_t x) {
    int32_t y = CLIP32(x);
    printf("%llx/%lld -> %x/%d\n", x, x, y, y);
    return 0;
}

int main() {
    int64_t a = 0x7FFFFFFFLL + 1LL;
    int64_t b = 0x7FFFFFFFLL;
    int64_t c = 0x0LL;
    int64_t d = -0x80000000LL;
    int64_t e = -0x80000000LL - 1LL;
    debug(a);
    debug(b);
    debug(c);
    debug(d);
    debug(e);
    return 0;
}

