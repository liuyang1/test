#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INVALID (-1)
int **initW(int n) {
    int **w = malloc(sizeof(int *) * n);
    int i;
    for (i = 0; i != n; i++) {
        int *p = malloc(sizeof(int) * n);
        int j;
        p[0] = i;
        for (j = 1; j != n; j++) {
            p[j] = INVALID;
        }
        w[i] = p;
    }
    return w;
}

void showW(int **w, int n) {
    int i, j;
    for (i = 0; i != n; i++) {
        printf("%2d:", i);
        int start = 1;
        for (j = 0; j != n; j++) {
            if (w[i][j] == INVALID) {
                break;
            }
            if (start) {
                printf(" ");
                start = 0;
            }
            printf(" %d", w[i][j]);
        }
        printf("\n");
    }
}

void deinitW(int **w, int n) {
    for (int i = 0; i != n; i++) {
        free(w[i]);
    }
    free(w);
}

void moveOnto(int **w, int n, int a, int b) {

}

void moveOver(int **w, int n, int a, int b) {

}

void pileOnto(int **w, int n, int a, int b) {

}

void pileOver(int **w, int n, int a, int b) {

}

#define CMDLEN      32
int main() {
    int n;
    int r = scanf("%d", &n);
    if (r != 1) {
        return -1;
    }
    int **w = initW(n);
    showW(w, n);
    char cmd[CMDLEN], dec[CMDLEN];
    int a, b;
    while ((r = scanf("%s%d%s%d", cmd, &a, dec, &b)) != 0) {
        if (strcmp(cmd, "quit") == 0) {
            break;
        }
        if (a == b) { // illegal comand
            continue;
        }
        printf("%s %d %s %d\n", cmd, a, dec, b);
    }
    printf("%s\n", cmd);
    deinitW(w, n);
    return 0;
}
