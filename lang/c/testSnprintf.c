#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fmtLog(char *pDst, size_t len, const char *fmt, va_list args) {
    return vsnprintf(pDst, len, fmt, args);
}

void test_fmtLog(const char *pExpect, const char *fmt, ...) {
#define LEN 16
    char *strbuf = malloc(sizeof(char) * LEN);
    va_list args;
    va_start(args, fmt);
    fmtLog(strbuf, LEN, fmt, args);
    va_end(args);

    printf("%s ?= %s %s\n",
           pExpect, strbuf, strcmp(pExpect, strbuf) == 0 ? "success" : "FAIL");
    free(strbuf);
}

void test(const char *fmt, ...)
{
    char s[LEN];
    va_list args;
    int ret = 0;
    va_start(args, fmt);
    ret = vsnprintf(s, LEN, fmt, args);
    // BUGGY, snprintf
    va_end(args);
    printf("%s -> %d\n", s, ret);
    if (ret > LEN - 1) {
        printf("exception\n");
    }
}

int main(int argc, char *argv[])
{
    test("%s %d", "01789", 123);
    test("%s", "def");
    test("%d", 1);

    test_fmtLog("01789 123", "%s %d", "01789", 123);
    return 0;
}
