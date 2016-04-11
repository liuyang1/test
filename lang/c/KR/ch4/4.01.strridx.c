#include "../kr.h"

/**
 * @brief return position of the RIGHTMOST occurrence of NEEDLE in HAYSTACK
 * or - 1 if there is none.
 */
int strridx(const char *haystack, const char *needle) {
    int len0 = strlen(haystack);
    int len1 = strlen(needle);
    int i, j;
    for (i = len0; i > len1; i--) {
        for (j = len1; j >= 0; j--) {
            if (haystack[i - j] != needle[len1 - j]) {
                break;
            }
        }
        if (j == 0) {
            return i - len1;
        }
    }
    return -1;
}

#define CASE(h, n, e) {int r = strridx(h, n);           \
                       printf("%s %s %d ?= %d -> %s\n", \
                              h, n, r, e, expect(r == e)); }

int main() {
    CASE("hello, world!", "world", 7);
    CASE("hello, world world!", "world", 13);
    CASE("hello, world!", "wrold", -1);
    return 0;
}
