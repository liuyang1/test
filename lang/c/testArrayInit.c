#include "stdio.h"
int main(int argc, char *argv[])
{
    int a[] = {[0] = 1, [2]= 3};
    printf("%d %d %d\n", a[0], a[1], a[2]);
    return 0;
}
