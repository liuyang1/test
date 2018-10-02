#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE  (10 * 1000 + 1) // consider range case
int smallestDiff(int *a, int n) {
    int *cnt = malloc(sizeof(int) * MAX_VALUE);
    memset(cnt, 0, sizeof(int) * MAX_VALUE);
    int i;
    for (i = 0; i != n; i++) {
        cnt[a[i]]++;
    }
    int m = 0;
    for (i = 0; i != MAX_VALUE - 1; i++) {
        int v = cnt[i] + cnt[i + 1];
        m = m < v ? v : m;
    }
    free(cnt);
    return m;
}

int main() {
    int T;
    scanf("%d", &T);
    for (; T != 0; T--) {
        int n;
        scanf("%d", &n);
        int i, *a = malloc(sizeof(int) * n);
        for (i = 0; i != n; i++) {
            scanf("%d", a + i);
        }
        int m = smallestDiff(a, n);
        printf("%d\n", m);
        free(a);
    }
    return 0;
}
