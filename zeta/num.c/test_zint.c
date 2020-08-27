#include <stdio.h>
#include "zint.h"

int test_ctor() {
    //char s[] = "169024356";
    char s[] = "16902";
    // char s[] = "1690";
    zint_t x = zint_ctor_str(s);
    zint_debug(x);
    // zint_show(x);
    return 0;
}

int test_add_pos() {
    uint8_t s0[] = {1, 9, 1};
    uint8_t s1[] = {9, 4, 8, 9};
    zint_t a = {.sign = 1, .s = s0, .n = sizeof(s0) / sizeof(uint8_t)};
    zint_t b = {.sign = 1, .s = s1, .n = sizeof(s1) / sizeof(uint8_t)};
    zint_t c = zint_add(a, b);
    zint_show(a), putchar('+'), zint_show(b), putchar('='), zint_show(c), printf("\n");
    zint_free(a), zint_free(b), zint_free(c);
    return 0;
}

int test_mul() {
    uint8_t s0[] = {1, 9, 1};
    uint8_t s1[] = {9, 4, 8, 9};
    zint_t a = {.sign = 1, .s = s0, .n = sizeof(s0) / sizeof(uint8_t)};
    zint_t b = {.sign = 1, .s = s1, .n = sizeof(s1) / sizeof(uint8_t)};
    zint_t c = zint_mul(a, b);
    zint_show(a), putchar('*'), zint_show(b), putchar('='), zint_show(c), printf("\n");
    zint_free(a), zint_free(b), zint_free(c);
    return 0;
}

int main() {
    test_ctor();
    // test_add_pos();
    // test_mul();
    return 0;
}

