/**
 * Well defined interface should handle all corner case, and defined all corner case.
 * This example show even a simple function `MODULAR`, still need design
 *
 * Ref: CIAI ch2.2
 */
#include <stdio.h>
#include <stdbool.h>

int mod_naive(int a, int b) {
    return a % b;
}

bool needAdjust(int a, int b) {
    // if division truncate toward 0
    // and A and B have different signs
    // and B doesn't divide A evenly
    return a < b * (a / b) &&
           (a < 0) != (b < 0) &&
           a % b != 0;
}

int div_arith(int a, int b) {
    if (needAdjust(a, b)) {
        return a / b - 1;
    } else {
        return a / b;
    }
}

int mod_arith(int a, int b) {
    return a - b * div_arith(a, b);
}

int mod_arith_1(int a, int b) {
    if (needAdjust(a, b)) {
        return a % b + b;
    } else {
        return a % b;
    }
}

#define EXPECT_INT_EQ(a_, b_) {              \
        int a = a_, b = b_;                  \
        if (a != b) {                        \
            printf("%d != %d fail\n", a, b); \
            return false;                    \
        }                                    \
}

bool test_mod(int (*MOD)(int, int)) {
    int n = 7;

    EXPECT_INT_EQ(MOD((n - 1) + 1, n), 0);
    EXPECT_INT_EQ(MOD(0 - 1, n), n - 1);
    return true;
}

#define CALL_TEST_MOD(func) {                                       \
        bool r = test_mod(func);                                    \
        printf("test_mod %-20s %s\n", # func, r ? "SUCC" : "FAIL"); \
}

int main() {
    CALL_TEST_MOD(mod_naive);
    CALL_TEST_MOD(mod_arith);
    CALL_TEST_MOD(mod_arith_1);
    return 0;
}
