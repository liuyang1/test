#include "base.h"
int func_a(int x) {
    return x * 2;
}

int func_depend_b(int x) {
    return func_b(x) + 2;
}
