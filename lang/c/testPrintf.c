#include <stdio.h>
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

int main()
{
    test_align();
    test_seq();
    test_var_width();
    test_repeat_char();
    return 0;
}
