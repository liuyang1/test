#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "maybe.h"

#define showBool(b) (b ? "success" : "FAIL")
#define EXPECT_STR(s, e) {                    \
        bool r = strcmp(s, e) == 0;           \
        printf("%s:%d \"%s\" ?= \"%s\" %s\n", \
               __FUNCTION__, __LINE__,        \
               s, e, showBool(r));            \
        if (!r) {                             \
            return r;                         \
        }                                     \
}
#define EXPECT_MAYBEINT(a, b) {                        \
        bool r = equal(a, b);                          \
        char sa[SHOWMAYBEINTLEN], sb[SHOWMAYBEINTLEN]; \
        printf("%s:%d %s ?= %s %s\n",                  \
               __FUNCTION__, __LINE__,                 \
               show(sa, a), show(sb, b), showBool(r)); \
        if (!r) {                                      \
            return r;                                  \
        }                                              \
}


int increment(int a) {
    return a + 1;
}

int triple(int a) {
    return a * 3;
}

bool test_show() {
    MaybeX v = JustInt(42);
    char s[SHOWMAYBEINTLEN];
    EXPECT_STR(show(s, v), "Just 42");

    v = NothingX();
    EXPECT_STR(show(s, v), "Nothing");

    v = JustInt(INT_MIN);
    EXPECT_STR(show(s, v), "Just -2147483648");

    v = JustFnc(triple);
    printf("%s\n", show(s, v));
    return true;
}

bool test_fmap() {
    MaybeX v = fmap(increment, JustInt(42));
    EXPECT_MAYBEINT(v, JustInt(43));

    v = fmap(triple, NothingX());
    EXPECT_MAYBEINT(v, NothingX());
    return true;
}

int test_Applicative() {
    MaybeX v = Applicative(JustFnc(triple), JustInt(42));
    EXPECT_MAYBEINT(v, JustInt(3 * 42));

    v = Applicative(JustFnc(triple), JustFnc(increment));
    EXPECT_MAYBEINT(v, NothingX());

    v = Applicative(JustFnc(triple), NothingX());
    EXPECT_MAYBEINT(v, NothingX());

    v = Applicative(NothingX(), JustInt(42));
    EXPECT_MAYBEINT(v, NothingX());
    return true;
}

int main() {
    test_show();
    test_fmap();
    test_Applicative();
    return 0;
}
