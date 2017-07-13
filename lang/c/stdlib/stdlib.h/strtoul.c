#include <stdio.h>
#include <stdlib.h>
// strtoul bug
void test(char *s)
{
    int i=strtoul(s, 0, 0);
    long l=strtoul(s, 0, 0);
    unsigned int ui=strtoul(s, 0, 0);
    unsigned long ul=strtoul(s, 0, 0);

    printf("%d %ld %u %lu\n", i, l, ui, ul);
}
void test2(char *s)
{
    int i=(int)strtoul(s, 0, 0);
    long l=(long)strtoul(s, 0, 0);
    unsigned int ui=(unsigned int)strtoul(s, 0, 0);
    unsigned long ul=(unsigned long)strtoul(s, 0, 0);

    printf("%d %ld %u %lu\n", i, l, ui, ul);
}
void test3(char *s)
{
    int i=(int)atoi(s);
    long l=(long)atol(s);
    unsigned int ui=(unsigned int)strtoul(s, 0, 0);
    unsigned long ul=(unsigned long)strtoul(s, 0, 0);

    printf("%d %ld %u %lu\n", i, l, ui, ul);
}
int testloop(void (*fn)(char*))
{
    char s[][20] = {
        "abc",
        "1000",
        "0xff",
        "0x1000",
        "01000",
        "10000000",
        "1000000000",
        "2000000000",
        "4000000000",
        "8000000000",
        "10000000000",
        "100000000000",
        "1000000000000000",
        "123 abc",
    };
    unsigned int i;
    for (i = 0; i < sizeof(s)/sizeof(s[0]); i++) {
        fn(s[i]);
    }
}

/** NOTES
 * - when strtoul success, it return [0, ULONG_MAX];
 * - when strtoul fail, it return 0 or ULONG_MAX, and setting errno;
 *   so if strtoul return 0 or ULONG_MAX, we must check errno, and have to setting it to zero before
 *   calling;
 *   also, need check endptr return value.
 */
/** COMMENT
 * This is a bad design style, It's so hard for user to write correctly program.
 */
#include <errno.h>
#include <string.h>
void testFail(char *s) {
    errno = 0;
    char *endptr = NULL;
    unsigned long i = strtoul(s, &endptr, 0);

    printf("%lu \t\t errno:%d(%s)\n", i, errno, strerror(errno));
    if (errno != 0) {
        printf("detec error\n");
    }

    printf("nptr=%p endptr=%p diff=%ld\n", s, endptr, endptr - s);
    printf(">>> ");
    if (*endptr == '\0') {
        printf("successfully parse it to end of string\n");
    } else if(endptr == s) {
        printf("nothing to parse\n");
    } else {
        printf("cannot parse when to endptr\n");
    }
}

int main(int argc, char *argv[])
{
    testloop(test);
    testloop(test2);
    testloop(test3);

    testFail("123");
    testFail("abc");
    testFail("12345678900000000000000000000");
    return 0;
}
