#include <stdio.h>

int main() {
    unsigned int x;
    scanf("%u", &x);
    printf("%s\n", (x % 2 == 0 && x != 2) ? "YES" : "NO");
    return 0;
}
