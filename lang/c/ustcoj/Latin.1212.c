#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isLatin(int **p, int n) {
    int *flagRow = malloc(sizeof(int) * n);
    int *flagCol = malloc(sizeof(int) * n);
    int i, j;
    bool ret = true;
    for (i = 0; i != n; i++) {
        for (j = 0; j != n; j++) {
            flagRow[j] = false;
            flagCol[j] = false;
        }
        for (j = 0; j != n; j++) {
            if (flagRow[p[i][j] - 1] || flagCol[p[j][i] - 1]) {
                ret = false;
                goto end;
            }
            flagRow[p[i][j] - 1] = true;
            flagCol[p[j][i] - 1] = true;
        }
    }
end:
    free(flagRow);
    free(flagCol);
    return ret;
}

bool isStdLatin(int **p, int n) {
    int i;
    for (i = 0; i != n; i++) {
        if (p[0][i] - 1 != i || p[i][0] - 1 != i) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
        if (n == 0) {
            break;
        }
        int **p = malloc(sizeof(int *) * n);
        int i, j;
        for (i = 0; i != n; i++) {
            p[i] = malloc(sizeof(int) * n);
            for (j = 0; j != n; j++) {
                scanf("%d", &p[i][j]);
            }
        }
        if (isLatin(p, n)) {
            if (isStdLatin(p, n)) {
                printf("%d\n", 2);
            } else {
                printf("%d\n", 1);
            }
        } else {
            printf("%d\n", 0);
        }
        for (i = 0; i != n; i++) {
            free(p[i]);
        }
        free(p);
    }
    return 0;
}
