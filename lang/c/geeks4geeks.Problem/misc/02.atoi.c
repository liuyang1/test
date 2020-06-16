/** ref:
 * - practice.geeksforgeeks.org/problems/implement-atoi/1
 * - cplusplus.com/reference/cstdlib/atoi
 */
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

int my_atoi(const char *nptr) {
    assert(nptr);
    int x = 0, base = 10;
    for (; isspace(*nptr); nptr++) {
    }
    int sign = 1; // default
    switch (*nptr) {
        case '+': sign = 1; nptr++; break;
        case '-': sign = -1; nptr++; break;
        case '\0': return 0;
    }
    for (; isdigit(*nptr); nptr++) {
        x = base * x + (*nptr - '0');
    }
    x *= sign;
    return x;
}

// #define DEBUG printf
#define DEBUG
bool unit_atoi(const char *s) {
    DEBUG("test on s=%s\n", s);
    int x = atoi(s);
    DEBUG("expect x=%d\n", x);
    int y = my_atoi(s);
    if (x != y) {
        printf("s=%s expect %d return %d\n", s, x, y);
    }
    return x == y;
}

bool test_atoi() {
    // unit_atoi(NULL);
    unit_atoi("\0");
    unit_atoi("123");
    unit_atoi("+123");
    unit_atoi("-123");
    unit_atoi("123a");
    unit_atoi("000123");
    unit_atoi("   123");
}

int main() {
    test_atoi();
    return 0;
}
