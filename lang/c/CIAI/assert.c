#include "assert.h"

const Except_T Assert_Failed = { "Assertion Failed"};

// why need this parentheses?
// suspress expansion of the macro `assert` and thus define the function,
// as required by the interface
void (assert)(int e) {
    assert(e);
}
