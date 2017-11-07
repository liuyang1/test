/** ref:
 * - http://www.mouseos.com/assembly/part2/strlen.html
 *   whole strlen algo check this page
 * - http://www.lrdev.com/lr/c/strlen.c
 * - http://tsunanet.net/~tsuna/strlen.c.html
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// is 0x00 in this int32_t.
// with this function, we could search string by sizeof(int) per step,
// instead of one char per step as naive style.
bool isZeroByte(int32_t v) {
    int32_t a = v + 0x7efefeff; // add magic
    int32_t b = ~v;
    int32_t c = (a ^ b) & 0x81010100;
    return c != 0;
}

#define showBool(b) (b ? "true" : "false")
#define CHECK_BOOL_EQ(a_, b_) {                                    \
        bool a = a_, b = b_;                                       \
        printf("%s ?= %s = %s %s\n",                               \
               # a_, # b_, showBool(b), a == b ? "SUCC" : "FAIL"); \
        if (a != b) {                                              \
            return false;                                          \
        }                                                          \
}
int main() {
    CHECK_BOOL_EQ(true, isZeroByte(0x31006261));
    CHECK_BOOL_EQ(false, isZeroByte(0xdeadbeaf));
    CHECK_BOOL_EQ(false, isZeroByte(0x80808080)); // false candidate result
    return 0;
}
