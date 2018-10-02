#include <stdio.h>
#include <assert.h>
#include <math.h>

int main() {
    int T;
    scanf("%d", &T);
    for (; T != 0; T--) {
        int surface;
        scanf("%d", &surface);
        surface /= 6;
        int edge = sqrt(surface);
        printf("%d\n", edge);
    }
    return 0;
}
