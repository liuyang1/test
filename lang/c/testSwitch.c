#include <stdio.h>

int main()
{
    unsigned int c = 1;
    switch(c){
        case 0:
        printf("case 0\n");
        break;
        case -1:
        printf("case 1\n");
        break;
        default:
        printf("default\n");
    };
    return 0;
}
