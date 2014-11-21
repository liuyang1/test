#include "stdarg.h"
#include "stdio.h"
void test(const char* fmt, ...)
{
#define LEN     16
    char s[LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(s, LEN, "%s", args);
    va_end(args);
    puts(s);
}
int main(int argc, char* argv[])
{
    test("%s", "01234567890123456789");
    return 0;
}
