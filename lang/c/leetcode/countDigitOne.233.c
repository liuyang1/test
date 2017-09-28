long long countDigitOne(long long n) {
    if (n <= 0) {
        return 0;
    } else if (n < 10) {
        return 1;
    }
    long long first, x = n;
    long long next = 1;
    while (x) {
        first = x % 10;
        x /= 10;
        next *= 10;
    }
    next /= 10;
    if (first == 1) {
        return (n - next + 1) + countDigitOne(next - 1) +
               countDigitOne(n - next);
    } else {
        long long nn = next * 2 - 1;
        return countDigitOne(n - first * next) +
               (first - 2) * countDigitOne(next - 1) +
               countDigitOne(nn);
    }
}

#include <stdio.h>
#define CASE(x, expect)     {                \
        long long _x = countDigitOne(x);     \
        printf("%lld %lld ?= %lld %s\n",     \
               x, _x, expect, expect == _x ? \
               "PASS" : "fail"); }

int main()
{
    CASE(-1ll, 0);
    CASE(0, 0);
    CASE(1, 1);
    CASE(9, 1);
    CASE(10, 2);
    CASE(16, 9);
    CASE(23, 13);
    CASE(123, 57);
    CASE(98754, 49656);
    CASE(1234567, 1058985);
    CASE(123456789, 130589849);
    CASE(1410065408, 1737167499);
    return 0;
}
