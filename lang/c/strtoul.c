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
    };
    unsigned int i;
    for (i = 0; i < sizeof(s)/sizeof(s[0]); i++) {
        fn(s[i]);
    }
}
int main(int argc, char *argv[])
{
    testloop(test);
    testloop(test2);
    testloop(test3);
    return 0;
}
