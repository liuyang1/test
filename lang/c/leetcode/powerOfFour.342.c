#include "leet.h"

bool isPowerOfFourBit(int num) {
    /**
     * - num > 0
     * - (num & (num - 1)) == 0     only have 1bit one
     * - (num & 0x55555555) != 0    the one bit must at odd postion
     */
    return num > 0 && (num & (num - 1)) == 0 && (num & 0x55555555) != 0;
}

bool isPowerOfFour(int num) {
    if (num <= 0) {
        return false;
    }
    if ((num & 0xffff) == 0) {
        num >>= 16;
    }
    if ((num & 0xff) == 0) {
        num >>= 8;
    }
    if ((num & 0xf) == 0) {
        num >>= 4;
    }
    if ((num & 0x3) == 0) {
        num >>= 2;
    }
    return num == 1;
}

bool unit(int num, bool e) {
    bool r = isPowerOfFour(num);
    if (r != e) {
        printf("isPowerOfFour(%d) = %s ?= %s %s\n",
               num, SBOOL(r), SBOOL(e), expect(false));
    }
    return r != e;
}

int main() {
    unit(0, false);
    unit(1, true);
    unit(16, true);
    unit(32, false);
    unit(256, true);
    unit(-16, false);
    return 0;
}
