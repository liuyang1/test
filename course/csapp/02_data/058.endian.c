#include <stdio.h>

#define XVAL       1
int is_little_endian() {
    int x = XVAL;
    char *p = (char *)&x;
    return *p == XVAL;
}

int main() {
    printf("%s endian\n",
           is_little_endian() ? "little" : "big");
    return 0;
}
