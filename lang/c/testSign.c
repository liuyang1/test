#include <stdio.h>
#define ERR -1
#define ERR2 0xffffffff
#define ERRU 0xffffffffu

int main(void)
{
    unsigned int a = ERR;
    int b = ERR;
    unsigned int uc = ERR2;
    int ic = ERR2;
    unsigned int ud = ERRU;
    int id = ERRU;

    printf("%x\n", a);
    if (a == -2) {
        printf("test compare\n");
    }
    return 0;
}
