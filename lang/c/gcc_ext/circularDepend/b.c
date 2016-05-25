#include "base.h"
int func_b(int x) {
    return x * 3;
}

int func_depend_a(int x) {
    return func_a(x) + 3;
}
