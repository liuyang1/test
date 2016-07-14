#include <stdio.h>
char* func() {
    printf("%s\n", __FUNCTION__);
    return NULL;
    // cannot return function name
    // return __FUNCTION__ "unknown";
    // return "unknown " __func__;
}

int main() {
    printf("%p\n", func());
    return 0;
}
