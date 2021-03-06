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

/**
 * %A.Bs
 * A means fixed width
 * B means length
 * 0, assume, the string's length is L
 * 1, Any string, it will truncate to B length first;
 * => L1 = min(L, B), B is Inf when undefined
 * 2, output it to fixed width A (if possible)
 * => L2 = max(L1, A), A is 0 when undefined
 *
 * L2 = max(min(L, B), A)
 */

static inline int min(int a, int b) {
    return a < b ? a : b;
}
static inline int max(int a, int b) {
    return a > b ? a : b;
}
static inline int length(int L, int A, int B) {
    return max(min(L, B), A);
}
int unit_fixed_length(char *str, int A, int B) {
    printf("'%*.*s'\n", A, B, str);
    int length = strlen(str);
    int content = min(length, B);
    int width = max(content, A);
    printf("length=%d content=%d width=%d\n", length, content, width);
    return 0;
}

int test_fixed_length(void) {
    unit_fixed_length("01", 10, 3);
    unit_fixed_length("0123", 10, 3);
    unit_fixed_length("013456789012", 10, 3);

    unit_fixed_length("01", 3, 10);
    unit_fixed_length("0123", 3, 10);
    unit_fixed_length("013456789012", 3, 10);
    unit_fixed_length("01", 3, 3);
    unit_fixed_length("0123", 3, 3);
    unit_fixed_length("013456789012", 3, 3);
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

int test_sharp() {
    printf("\n> flag char # converted the representation to alternative form\n"
           "> - o\n"
           ">\tadd prefix 0 if it was not zero already.\n"
           "> - x, X\n"
           ">\tadd prefix 0x if it was not zero already.\n"
           "> - a, A, e, E, f, F\n"
           ">\talways contain a decimal point, even if not digits follow it.\n"
           "> - g, G\n"
           ">\tsame as \"a, A, ...\", but not remove trailing zeros\n");

    printf("\n");

    int a = 64;
    printf("%d: %x %#x\n", a, a, a);
    a = 0;
    printf("%d: %x %#x\n", a, a, a);
    printf("> REMIND: it don't add leading \'0x\' decorator for zero.\n");
    a = -1;
    printf("%d: %x %#x\n", a, a, a);

    printf("\n");

    double b = 1.0;
    printf("%f: %#f %#g\n", b, b, b);
    b = 2.1;
    printf("%f: %#f %#g\n", b, b, b);
    b = 1/3.0;
    printf("%f: %#f %#g\n", b, b, b);
    b = 100000 * 1000;
    printf("%f: %#f %#g\n", b, b, b);
    printf("> I don't find any difference with \'#\' flag.\n");
    return 0;
}

#include <inttypes.h>
int test_print_format() {
    uint32_t a = 42;
    printf("decimal: %d\n", a);
    printf("hex: %x\n", a);
    printf("hex: %#0x\n", a); // auto add 0x
    printf("hex: %#08x\n", a); // add 0 to padding length to 8
    return 0;
}

int main()
{
    test_print_format();
    test_null();
    test_align();
    test_seq();
    test_var_width();
    test_repeat_char();
    test_printf_ret();
    test_printf_len();
    test_sprintf();
    test_LogOutFile();
    test_sharp();
    test_fixed_length();
    return 0;
}
