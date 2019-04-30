#include <stdio.h>

int add(int x, int y) {
    return x + y;
}

/*
   - print file and lineno, which help debug
   - trace function
   - ({}) support value of statements, which is value of last statement;
 */
#define ADD_MONKEY(x, y) ({\
    printf("monkey patch at %s %d\n", __FILE__, __LINE__); \
    printf("in param: %d %d\n", x, y); \
    __typeof__(x) ret = add(x, y); \
    printf("ret: %d\n", ret); \
    ret; \
})

int main() {
    int a = 2, b = 3;
    int r;

    // same format, not need change code when call patched function
    r = add(a, b);
    r = ADD_MONKEY(a, b);
    printf("ret=%d\n", r);

    return 0;
}
