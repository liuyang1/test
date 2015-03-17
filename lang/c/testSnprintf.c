#include "stdarg.h"
#include "stdio.h"
void test(const char* fmt, ...)
{
#define LEN     20
    char s[LEN];
    va_list args;
    int ret = 0;
    va_start(args, fmt);
    ret = vsnprintf(s, LEN, "%s", args);
    va_end(args);
    printf("%s %d\n", s, ret);
    if (ret > LEN - 1) {
        printf("exception\n");
    }
}
int main(int argc, char* argv[])
{
    test("%s", "01234567890123456789");
    return 0;
}
