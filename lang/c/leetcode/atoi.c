#include <stdlib.h>
#include <ctype.h>
#define INT_MAX 2147483647
#define INT_MIN (-2147483648)

int myAtoi(char* str) {
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
        // printf("sign %d ret=%d val=%d\n", sign, ret, val);
        if (sign > 0 && ret > (INT_MAX - val) / 10) {
            return INT_MAX;
        } else if (sign < 0 && ret > -1 * (INT_MIN + val) / 10) {
            return INT_MIN;
        }
        ret = ret * 10 + val;
    }
    return sign * ret;
}

// leetcode web get different test result
#include <stdio.h>
int main()
{
    printf("%d\n", myAtoi(""));
    printf("%d\n", myAtoi("123"));
    printf("%d\n", myAtoi("   -123"));
    printf("%d\n", myAtoi("2147483647"));
    printf("%d\n", myAtoi("2147483648"));
    printf("%d\n", myAtoi("-2147483647"));
    printf("%d\n", myAtoi("-2147483648"));
    printf("%d\n", myAtoi("-2147483649"));
    printf("%d\n", myAtoi("-2147483650"));
    printf("%d\n", myAtoi("   -000134"));
    printf("%d\n", myAtoi(" -01324000"));
    printf("%d\n", myAtoi(" -1324000"));
    return 0;
}
