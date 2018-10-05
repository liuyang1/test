/**
 * Be careful about IO
 */
/** Solution:
 * MIN(n / 11, cnt of '8')
 */
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main() {
    int n;
    scanf("%d", &n);
    getchar(); /** eat the newline char for next getchar */
    int ecnt = 0, i;
    for (i = 0; i != n; i++) {
        char m = 'x';
        scanf("%c", &m);
        ecnt += m == '8';
    }
    n /= 11;
    // printf("n=%d ecnt=%d\n", n, ecnt);
    int r = MIN(n, ecnt);
    printf("%d\n", r);
    return 0;
}
