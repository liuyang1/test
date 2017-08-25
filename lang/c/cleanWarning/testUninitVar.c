#include <stdio.h>
int main() {
    printf("In C,\n"
           "- Uninitiliazed STATIC var, are guarnteed to start out as zero,\n"
           "    if typed = 0, {0}, NULL, or 0.0\n"
           "- Var with AUTO duration, start out containing garbage, unless they\n"
           "     explicitly initiliazed.\n"
           "\n"
           "This rule do apply to arrays and structs.\n"
           "When an AUTO array has a partial initiliazed, the remainder is initiliazed\n"
           "   to 0, just as for static\n"
           "\n"
           "For both cases, gcc DONOT consider them as warning.\n");
    int i;
    {
        int a[3] = {1, 2, 3};
        for (i = 0; i != 3; i++) {
            printf("%d\n", a[i]);
        }
    }

#ifdef UNINIT
    // If b is not inited, then we have garbage data. It's same with A in this test.
    int b[3];
#else
    // If b is inited, then we get correct value.
    // Partial initalized value is inited with 0;
    int b[3] = {10, 9};
#endif

    for (i = 0; i != 3; i++) {
        printf("%d\n", b[i]);
    }
    return 0;
}
