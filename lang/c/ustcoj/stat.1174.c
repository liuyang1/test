#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int v;
    int c;
} StatElem;
void showSt(StatElem *s, int n) {
    int i;
    char *lead = "";
    for (i = 0; i != n; i++) {
        printf("%s%d", lead, s[i].v);
        lead = " ";
    }
    printf("\n");
}
int cmp(const void *a, const void *b) {
    const StatElem *x = a;
    const StatElem *y = b;
    if (x->c != y->c) {
        return -1 * (x->c - y->c);
    }
    return x->v - y->v;
}
void stat(int *a, int n) {
    StatElem *s = malloc(sizeof(StatElem) * n);
    int i, j, k;
    for (i = 0, k = 0; i != n; i++) {
        j = 0;
        while (j != k) {
            if (a[i] == s[j].v) {
                s[j].c++;
                break;
            }
            j++;
        }
        if (j == k) { // not found
            s[k].v = a[i];
            s[k].c = 1;
            k++;
        }
    }
    qsort(s, k, sizeof(s[0]), cmp);
    showSt(s, k);
}
#define NUM 101
int main() {
    int a[NUM];
    char *s = NULL;
    size_t m = 0;
    while (getline(&s, &m, stdin) > 0) {
        char *t = s;
        int i;
        for (i = 0; i != NUM; i++) {
            char *p = strtok(t, " ");
            if (p == NULL) {
                break;
            }
            t = NULL;
            a[i] = atoi(p);
        }
        stat(a, i);
    }
    if (s != NULL) {
        free(s);
    }
    return 0;
}
