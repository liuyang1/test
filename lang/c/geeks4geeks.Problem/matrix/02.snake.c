/** print square matrix with snake pattern */
/** 0: beginner */

#include <stdio.h>
#include <stdlib.h>

void show_snake(int *m, int n) {
    int i, j;
    for (i = 0; i != n; i++) {
        if (i % 2 == 0) {
            for (j = 0; j != n; j++) {
                printf("%d ", m[i * n + j]);
            }
        } else {
            for (j = n - 1; j >= 0; j--) {
                printf("%d ", m[i * n + j]);
            }
        }
    }
    printf("\n");
}

void show_mat(int *m, int n) {
    int i, j;
    for (i = 0; i != n; i++) {
        for (j = 0; j != n; j++) {
            printf("%d ", m[i * n + j]);
        }
        printf("\n");
    }
}

int main() {
    int t;
    int *m, n, i, n2;
    for (scanf("%d", &t); t != 0; t--) {
        for (scanf("%d", &n), n2 = n * n, m = malloc(sizeof(int) * n2), i = 0;
             i < n2; i++) {
            scanf("%d", &m[i]);
        }
        // show_mat(m, n);
        show_snake(m, n);
        free(m);
    }
    return 0;
}
