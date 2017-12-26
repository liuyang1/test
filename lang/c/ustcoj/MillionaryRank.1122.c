#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define K 10
char *top[K] = {NULL};

void showTop() {
    int i;
    for (i = 0; i != K; i++) {
        printf("%s\n", top[K - i - 1]);
    }
}

void skipZeros(char *s) {
    char *p = s;
    while (*p == '0') {
        p++;
    }
    memmove(s, p, strlen(p) + 1);
}

int cmp(char *a, char *b) {
    if (a == NULL && b == NULL) {
        return 0;
    }
    if (a == NULL) {
        return -1;
    }
    if (b == NULL) {
        return 1;
    }
    int la = strlen(a), lb = strlen(b);
    if (la == lb) {
        return strcmp(a, b);
    }
    return (la > lb) - (la < lb);
}

void putRank(char *s) {
    int i;
    if (cmp(s, top[0]) <= 0) {
        return;
    }
    top[0] = s;
    for (i = 1; i != K; i++) {
        if (cmp(top[i - 1], top[i]) > 0) {
            char *t = top[i - 1];
            top[i - 1] = top[i];
            top[i] = t;
        } else {
            break;
        }
    }
}

int main() {
    int n, i;
    scanf("%d", &n);
    getchar();
    char **a = malloc(sizeof(char *) * n);
    for (i = 0; i != n; i++) {
        a[i] = NULL;
    }
    for (i = 0; i != n; i++) {
        size_t len = 0;
        if (getline(&a[i], &len, stdin) <= 0) {
            break; // find execpt
        }
        skipZeros(a[i]); // remove newline
        a[i][strlen(a[i]) - 1] = '\0';
        putRank(a[i]);
    }
    showTop();
    for (i = 0; i != n; i++) {
        if (a[i] != NULL) {
            free(a[i]);
        }
    }
    free(a);
    return 0;
}
