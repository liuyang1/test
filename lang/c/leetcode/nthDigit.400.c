#include "leet.h"

int nthDigit(int n, int idx) {
    char s[20];
    // stupid use sprintf to get nth digit of n
    sprintf(s, "%d", n);
    return s[idx - 1] - '0';
}

int findNthDigit(int n) {
    int width, len, num;
    for (width = num = 1, len = 9;
         n > width * len;
         width++, len *= 10, num *= 10) {
        int m = width * len;
        if (m / width == len) {
            n -= width * len;
        } else {
            // overflow at here
            // if find overflow, it must over this width region, just break
            break;
        }
    }
    for (; n > width; n -= width) {
        num++;
    }
    return nthDigit(num, n);
}

bool unit(int n, int e) {
    int r = findNthDigit(n);
    bool ret = r == e;
    if (!ret) {
        printf("findNthDigit(%d) = %d != %d %s\n",
               n, r, e, expect(ret));
    }
    return ret;
}

int main() {
    unit(3, 3);
    unit(9, 9);
    unit(11, 0);
    unit(100, 5);
    unit(1000, 3);
    unit(10000, 7);
    unit(100000, 2);
    unit(1000000, 1);
    unit(2147483647, 2);
    return 0;
}
