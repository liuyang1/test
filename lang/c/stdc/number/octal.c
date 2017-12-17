#include "stdio.h"
int main(int argc, char *argv[])
{
    printf("%d\n", 0020);
    // printf("%d\n", 008); // invalid
    printf("%d\n", 007);
    return 0;
}
