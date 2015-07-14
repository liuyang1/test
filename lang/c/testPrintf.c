#include <stdio.h>
#define TAG "012345"
#define PID 3593
int main(void)
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
