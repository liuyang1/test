#include <stdio.h>

void f(void);

int x = 1;

int main() {
    printf("before call f %d\n", x);
    f();
    printf("after call f  %d\n", x);

    return 0;
}
