#include <stdio.h>

int main()
{
    unsigned int n = -1;
    printf("unsigned int:%u\nhex: %x\nint: %d\n", n, n, n);
    printf("compare with unsigned number and signed nubmer\n");
    if (n == -1) {
        printf("yes\n");
    } else {
        printf("no\n");
    }
    return 0;
}
