/*
   The function first discards as many whitespace characters as necessary until
   the first non-whitespace character is found.
   Then, starting from this character, takes an optional initial plus or minus
   sign followed by as many numerical digits as possible, and interprets them as a
   numerical value.

   The string can contain additional characters after those that form the integral
   number, which are ignored and have no effect on the behavior of this function.

   If the first sequence of non-whitespace characters in str is not a valid
   integral number, or if no such sequence exists because either str is empty or
   it contains only whitespace characters, no conversion is performed.

   If no valid conversion could be performed, a zero value is returned.
   If the correct value is out of the range of representable values,
   INT_MAX (2147483647) or INT_MIN (-2147483648) is returned.
 */
#include "leet.h"
#define INT_MAX 2147483647
#define INT_MIN (-2147483648)

int myAtoi(char *str) {
    while (isspace(*str)) {
        str++;
    }
    int sign = 1;
    if (*str == '+') {
        str++;
        sign = 1;
    } else if (*str == '-') {
        str++;
        sign = -1;
    }
    char *beg;
    int ret = 0, val;
    for (beg = str; isdigit(*beg); beg++) {
        val = *beg - '0';
        if (sign > 0 && ret > (INT_MAX - val) / 10) {
            return INT_MAX;
        } else if (sign < 0 && ret > -1 * (INT_MIN + val) / 10) {
            return INT_MIN;
        }
        ret = ret * 10 + val;
    }
    return sign * ret;
}

// test code
#define CASE(tag, s) {int e = atoi(s), r = myAtoi(s); \
                      printf("%10s %s %d ?= %d %s\n", tag, s, r, e, expect(r == e)); }

int main()
{
    CASE("space", "");
    CASE("space", " ");
    CASE("space", "  ");
    CASE("space", "  123");
    CASE("sign", "+123");
    CASE("sign", "-123");
    CASE("zero-lead", "0123");
    CASE("zero-lead", "+0123");
    CASE("zero-lead", "-0123");
    CASE("max", "2147483647");
    CASE("min", "-2147483648");
    printf("leetcode handle overflow problem is different with system\n");
    CASE("overflow", "2147483648");
    CASE("overflow", "-2147483649");
    return 0;
}
