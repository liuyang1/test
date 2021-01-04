#include "leet.h"

int main() {
    int a[] = {1,2, 3, 4};
    showArr(a, 4, "\n");

    char str[] = "hello, world";
    showArr(str, sizeof(str) / sizeof(str[0]), "\n");
    return 0;
}
