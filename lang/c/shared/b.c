#include "a.h"
#include "third.h"

int func_b(int x) {
    return func_a(x) * 3 * func_third(x);
}
