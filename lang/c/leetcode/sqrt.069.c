#include "leet.h"
int mySqrt(int x) {
    if (x < 0) {
        return 65535;
    }
    if (x <= 1) {
        return x;
    }
    unsigned long long b, e, m, m2, m12;
    for (b = 1, e = 65535; b <= e; ) {
        m = (b + e) / 2;
        m2 = m * m;
        m12 = m2 + 2 *m + 1;
        if (m2 <= x && x < m12) {
            return m;
        } else if (m2 < x) {
            b = m + 1;
        } else {
            e = m - 1;
        }
    }
    return -1;
}

#define CASE(x) {int r = mySqrt(x); \
    printf("%d %d %s\n", \
           x, r, expect(r * r <= x && x < (r + 1) * (r + 1)));}  \

int main() {
    int i;
    for (i = 0; i < 26; i++) {
        CASE(i);
    }
    return 0;
}

