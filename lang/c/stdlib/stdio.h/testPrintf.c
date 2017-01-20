#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <strings.h>
#include <string.h>
#define TAG "012345"
#define PID 3593

int test_align(void)
{
    printf("%-8s(%5d)\n", TAG, PID);
    // left align
    printf("%-8s\n", "12345");
    // right align
    printf("%8s\n", "12345");
    // when output string longer than indicated length,
    // it will output full string.
    printf("%-8s\n", "0123456789");
    printf("%8s\n", "0123456789");
    return 0;
}

int test_seq(void)
{
    char weekday[] = "Mon";
    char month[] = "Nov";
    int day = 14, hour = 15, min = 46;

    char fmt0[] = "%s, %s %d, %.2d:%.2d\n";
    printf(fmt0, weekday, month, day, hour, min);

    // %m$
    // decimal integer M denotes the postion in the argument list of desired
    // argument, indexed strating from 1.
    // C99 standard does NOT include the style using `$`.
    //
    // switch to another format, but with same arguments sequence
    // This feature is seen in Python, but C also support it.
    char fmt1[] = "%1$s, %3$d. %2$s, %4$d:%5$.2d\n";
    printf(fmt1, weekday, month, day, hour, min);
    return 0;
}

int test_var_width()
{
    int width = 10;
    printf("%*d\n", width, 4);
    printf("%*s\n", width, "abc");
    printf("%2$*1$s\n", width, "abc");
    // switch position
    printf("%1$*2$s\n", "abc", width);
    return 0;
}

int test_repeat_char()
{
    int width = 10;
    // only work repeat 0 or whitespace
    printf("%0*d\n", width, 0);
    printf("%*d\n", width, 0);
    // better, but limit length
#define BAR "========="
    printf("[%.*s]\n", 0, BAR);
    printf("[%.*s]\n", 5, BAR);
    printf("[%.*s]\n", 20, BAR);
    return 0;
}

int printf_len(int srclen, int dstlen) {
    char *buf = malloc(sizeof(char) * dstlen);
    int len = snprintf(buf, dstlen, "%.*s", srclen, "=====================");
    printf("%2d %d -> %d %s\n", srclen, dstlen, len, buf);
    free(buf);
    return 0;
}

int test_printf_len() {
    printf_len(0, 10);
    printf_len(1, 10);
    printf_len(9, 10);
    printf_len(10, 10);
    printf_len(11, 10);
    return 0;
}

int test_printf_ret()
{
    char str[] = "";
    int len = printf("%s", str);
    printf("%s %d\n", str, len);

#define LEN 10
    char buf[LEN];
    len = snprintf(buf, LEN, "%s", str);
    printf("%s %d\n", str, len);
    return 0;
}

int test_sprintf()
{
    char str[1024] = "abc";
    // undefine behaviour
    sprintf(str, "%s%s", "def", str);
    printf("%s\n", str);
    return 0;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void fmtLog(char *pDst, size_t len, const char *pHead, const char *pMsg, va_list args) {
    int pos = 0;
    if (pHead) {
        pos += snprintf(pDst + pos, len - pos, "[%s] ", pHead);
    }
    pos += vsnprintf(pDst + pos, len - pos, pMsg, args);
    // snprintf or vsnprintf return value is number of char (excluding null),
    // which would have written to final string if ENOUGH space had been available.
    // so it may return ret > LEN
    pos = MIN(pos, len - 2);
    if (pDst[pos - 1] != '\n') {
        pDst[pos] = '\n';
        pDst[pos + 1] = '\0';
    }
}

int case_LogOutFile(const char *pExpect, const char *pHead, const char *pFmt, ...) {
#undef LEN
#define LEN 16
    char *strbuf = malloc(sizeof(char) * LEN);

    va_list args;
    va_start(args, pFmt);
    fmtLog(strbuf, LEN, pHead, pFmt, args);
    va_end(args);

    fprintf(stdout, "{%s}\n{%s} %s\n\n",
            pExpect, strbuf, strcmp(pExpect, strbuf) == 0 ? "sucess" : "FAIL");
    free(strbuf);

    return 0;
}

int test_LogOutFile() {
    case_LogOutFile("[abc] def\n", "abc", "def");
    case_LogOutFile("[] def\n", "", "def");
    case_LogOutFile("def\n", NULL, "%s", "def");
    case_LogOutFile("1 ff\n", NULL, "%d %x", 1, 255);
    case_LogOutFile("1.000000\n", NULL, "%f", 1.0);
    case_LogOutFile("with newline\n", NULL, "with newline\n"); // newline + overflow
    case_LogOutFile("new\n", NULL, "new\n"); // newline
    case_LogOutFile("0123456789012\n", NULL, "0123456789012");
    case_LogOutFile("01234567890123\n", NULL, "01234567890123");
    case_LogOutFile("01234567890123\n", NULL, "012345678901234");
    case_LogOutFile("new\n\n", NULL, "new\n\n"); // double newline
    return 0;
}

int test_null() {
    printf("%p\n", NULL); // (nil)
    // printf("%s\n", NULL); // crash
    return 0;
}

int main()
{
    test_null();
    test_align();
    test_seq();
    test_var_width();
    test_repeat_char();
    test_printf_ret();
    test_printf_len();
    test_sprintf();
    test_LogOutFile();
    return 0;
}
