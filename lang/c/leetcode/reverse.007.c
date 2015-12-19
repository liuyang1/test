#define MAXINT (0x7fffffff)

int reverse(int x) {
    if (x < 0) {
        if (-1 * ((long long)x) > MAXINT) {
            return 0;
        }
        return -1 * reverse(-1 * x);
    }
    if (x == 0) {
        return 0;
    }
    int ret = 0;
    while (x > 0) {
        // check overflow
        if (ret > (MAXINT - x % 10) / 10) {
            return 0;
        }
        ret = ret * 10 + (x % 10);
        x = x / 10;
    }
    return ret;
}
