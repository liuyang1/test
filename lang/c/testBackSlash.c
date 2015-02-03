#include <stdio.h>

#define PRINT(x) printf(x)
int main(void)
{
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
