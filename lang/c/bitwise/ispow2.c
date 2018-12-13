#include <stdio.h>

#define ISPOW2(x) (((x) & ((x) - 1)) == 0)

#define UNIT(x) printf("%d %s\n", x, ISPOW2(x) ? "true" : "false")
int main() {
    int i;
    for (i = 0; i != 16; i++) {
        UNIT(i);
    }
    return 0;
}
