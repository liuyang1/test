#include <stdio.h>

void __f() {
    printf("at func=%s\n", __FUNCTION__);
}

// directly user weak alias attribute on function
void f() __attribute__ ((weak, alias("__f")));

// use it with MACRO
#define WEAK_ALIAS(name, aliasname) \
    extern typeof(name) aliasname __attribute((weak, alias(# name)));

WEAK_ALIAS(__f, g);

int main() {
    f();
    g();
    return 0;
}
