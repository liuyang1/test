#include <stdio.h>

int main()
{
    int idx = 16;
    switch(idx) {
        case 0 ... 16:
            printf("first group case: idx=%d\n", idx);
            break;
        case 17 ... 32:
            printf("second group case: idx=%d\n", idx);
            break;
        default:
            printf("default idx=%d\n", idx);
            break;
    }
    return 0;
}
