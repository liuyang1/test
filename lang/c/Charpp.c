#include "stdio.h"
int main(int argc, char* argv[])
{
    char* str[] = {"012", "0123456", "0123", "0123"};

    size_t i;
    for (i = 0; i < sizeof(str) / sizeof(str[0]); ++i) {
        printf("%p %lu %s\n", str[i], sizeof(str[i]), str[i]);
    }
    printf("%lu\n", sizeof(str) / sizeof(str[0]));
    char** p = str;
    for(p; p)

    return 0;
}
