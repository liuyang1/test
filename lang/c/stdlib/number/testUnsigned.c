#include <stdio.h>
int main()
{
    unsigned int t = -1;
    printf("%d %u 0x%x\n", t, t, t);
    if((size_t)t > 100) {
        printf("error\n");
    }else{
        printf("right\n");
    }
    return 0;
}
