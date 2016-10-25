#include "leet.h"

bool isPerfectSquare(int num) {
    if (num < 0) {
        return false;
    }
    if (num == 0) {
        return true;
    }
    int b, e, m, s;
    for (b = 0, e = 65536; b <= e; ) {
        m = (b + e) / 2;
        s = m * m;
        if (s / m != m || s > num) {
            e = m - 1;
        } else if (s < num) {
            b = m + 1;
        } else {
            return true;
        }
    }
    return false;
}

bool unit(int num, bool e) {
    bool r = isPerfectSquare(num);
    bool ret = r == e;
    if (!ret) {
        printf("isPerfectSquare(%d) = %s ?= %s %s\n",
               num, SBOOL(r), SBOOL(e), expect(ret));
    }
    return ret;
}

int main() {
    int i, j, ns;
    for (i = 0, j = ns = 0; i != 10 * 1000 * 10; i++) {
        if (i == ns) {
            ns += 2 * j + 1;
            j++;
            // printf("i=%d j=%d ns=%d\n", i, j, ns);
            unit(i, true);
        } else {
            unit(i, false);
        }
    }
    unit(INT_MAX, false);
    unit(SHRT_MAX, false);
    unit(USHRT_MAX, false);
    unit(USHRT_MAX + 1, true);
    unit(2147395600, true);
    return 0;
}
