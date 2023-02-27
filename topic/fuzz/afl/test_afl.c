#include <stdio.h>
#include <stdlib.h>

int main() {
    int d;
    scanf("%d", &d);
    printf("input: %d\n", d);
    if (d == 42) {
        char *p = malloc(d); // leak when d == 42
        *p = 100;
        *(p - 1) = 100; // overwrite to wrong memory
    } else if (d == 4) {
	char *p = (char *)42;
	*p = 42; // crash
    }
    return 0;
}
