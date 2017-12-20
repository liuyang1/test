#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM 6
bool isConflict(int *a) {
    int i, j;
    for (i = 0; i != NUM; i++) {
        for (j = i + 1; j != NUM; j++) {
            if (a[i] == a[j]) {
                return false;
            }
            if (abs(a[i] - a[j]) == abs(i - j)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int i, j, n;
    scanf("%d", &n);
    int a[NUM];
    for (i = 0; i != n; i++) {
        for (j = 0; j != NUM; j++) {
            scanf("%d", &a[j]);
        }
        printf("%s\n", isConflict(a) ? "YES" : "NO");
    }
    return 0;
}
