#include <stdio.h>
#include <stdlib.h>

int main() {
    int d;
    scanf("%d", &d);
    printf("input: %d\n", d);
    if (d == 42) {
        char *p = malloc(d); // leak when d == 42
    }
    return 0;
}
