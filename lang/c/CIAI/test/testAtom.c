#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "atom.h"

#include "test.h"

bool testAtomSimple() {
    const char *s = Atom_string("abc");
    ASSERT(strcmp(s, "abc") == 0);

    s = Atom_int(1234);
    ASSERT(strcmp(s, "1234") == 0);

    s = Atom_new("abcdef", 3);
    ASSERT(strcmp(s, "abc") == 0);

    int n = Atom_length(s);
    ASSERT(n == 3);
    return true;
}

bool testAtomEq() {
    const char *s0 = Atom_string("abc");
    const char *s1 = Atom_string("abc");
    EXPECT_PTR_EQ(s0, s1);

    const char *s2 = Atom_string("def");
    ASSERT(s0 != s2);
    return true;
}

bool testAtom() {
    ASSERT(testAtomSimple());
    ASSERT(testAtomEq());
    return true;
}
