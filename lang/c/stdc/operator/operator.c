#include <stdio.h>

int main() {
    printf("%d\n", (7 & 8) - 1);
    // & > -
    printf("%d\n", 7 & 8 - 1);
    // but we still prefer to add ()
    printf("%d\n", 7 & (8 - 1));
    return 0;
}
