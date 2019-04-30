#include <stdio.h>

#define PRINT(...)  printf(__VA_ARGS__)
#define PRINTD(pMsg, ...) printf("%s" pMsg, s, __VA_ARGS__)
int main(void)
{
    char s[] = "test value ";
    PRINT("abc\n");
    PRINTD("abc %s\n", "more test");

    // build error
    // at least one value to extent for __VA_ARGS__
    // PRINTD("abc\n");
    return 0;
}
