#include "stdio.h"

int main()
{
    int a[] = {0, 1, 2};
    unsigned int i;
    for (i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
        if (a[i] == 3) {
            break;
        }
    }
    printf("%d\n", i);
    return 0;
}
