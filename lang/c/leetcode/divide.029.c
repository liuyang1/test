/**
 * use +, -, >>, << to caculate divide
 */
#include <stdio.h>
#include <limits.h>

/**
 * - this function for a, b < 0 version
 *   Why use negative number?
 *      The signed integer number is [-2^k, 2^k-1], k = 31
 *      so use [-2^k, 0] to represent all kinds
 * - b != 0
 *
 * Keep this invarint
 *     a = b * x + y
 *     x .., y = a / b
 *     x = divide_in(a, b, &y);
 */
long int divide_in(long int a, long int b, long int *py) {
    long int x = 0, y = a;
    if (a < b) {
        x = divide_in(a, b << 1, &y);
    }
    x = x << 1;
    if (y <= b) {
        x++;
        y -= b;
    }
    if (py) {
        *py = y;
    }
    return x;
}

// two's complement, so we must special handle INT_MIN case first
int divide(int dividend, int divisor) {
    if (divisor == 0) {
        return INT_MAX;
    }
    if (dividend == 0) {
        return 0;
    }
    if (dividend == INT_MIN) {
        if (divisor == -1) {
            return INT_MAX;
        }
        if (divisor == 1) {
            return INT_MIN;
        }
    }
    if (dividend > 0 && divisor > 0) {
        return divide(-dividend, -divisor);
    } else if (dividend < 0 && divisor < 0) {
        // directly skip in this case
    } else if (dividend < 0) {
        return -1 * divide(dividend, -divisor);
    } else {
        return -1 * divide(-dividend, divisor);
    }
    // divisor, dividend <= 0 at here
    return divide_in(dividend, divisor, NULL);
}

#define CASE(a, b) {int x = divide(a, b); int e = a / b; \
                    printf("%d/%d=%d ?= %d %s\n",        \
                           a, b, x, e, x == e ? "PASS" : "fail"); }
#define CASE_EX(a, b, e) {int x = divide(a, b);         \
                          printf("%d/%d=%d ?= %d %s\n", \
                                 a, b, x, e, x == e ? "PASS" : "fail"); }
int main()
{
    CASE(0, 1);
    CASE(100, 1);
    CASE(100, 2);
    CASE(32, 2);
    CASE(101, 2);
    CASE(10, 2);
    CASE(11, 2);
    CASE(-10, 2);
    CASE(-11, -2);
    CASE(1000000, 2);
    CASE(2147483647, -1);
    CASE(2147483647, -2);
    CASE(2147483647, -3);
    CASE(2147483647, 3);
    CASE(INT_MIN, 2);
    CASE_EX(INT_MIN, -1, INT_MAX);
    CASE_EX(INT_MIN, 1, INT_MIN);
    return 0;
}
