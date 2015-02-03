#include <stdio.h>
#define ERR -1

int main(void)
{
    unsigned int a = ERR;
    printf("%x\n", a);
    if (a == -2) {
        printf("test compare\n");
    }
    return 0;
}
