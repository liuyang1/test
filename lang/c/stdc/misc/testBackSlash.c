#include <stdio.h>

#define PRINT(x) printf(x)
int main(void)
{
    // in C source code, newline is ignore at most case.
    // but cannot add newline in literal string
    /* this style compile error */
    /*
    PRINT("test 
           abc\n");
    */
    PRINT("test \
           abc\n");
    PRINT("test"
           "abc\n");
    return 0;
}
