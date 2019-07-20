#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char c;
    int count;
} Entry;

void showArr(Entry *a, int n) {
    int i;
    for (i = 0; i != n; i++) {
        printf("%c%d ", a[i].c, a[i].count);
    }
}

int main() {
    int num = 1000 * 1000;
    Entry *a = malloc(sizeof(Entry) * num);
    memset(a, 0x0, sizeof(Entry) * num);
    int c, lastc = 0, i = 0;
    int cnt;
    while ((c = getchar()) != EOF) {
        // printf("find c=%c\n", c);
        if (c != lastc) {
            if (lastc == 'o') {
                Entry e = {.c = 'o', .count = cnt};
                a[i++] = e;
            } else if (lastc == 'v' && cnt > 1) {
                Entry e = {.c = 'w', .count = cnt - 1};
                a[i++] = e;
            }
            cnt = 1;
        } else {
            cnt++;
        }
        if (c == '\n') {
            break;
        }
        lastc = c;
    }

    showArr(a, i);

    int j, wcnt, pcnt;
    for (j = wcnt = 0; j != i; j++) {
        if (a[j].c == 'w') {
            wcnt += a[j].count;
        }
    }
    int s;
    for (j = pcnt = s = 0; j != i; j++) {
        if (a[j].c == 'w') {
            pcnt += a[j].count;
        }
        if (a[j].c == 'o') {
            s += a[j].count * pcnt * (wcnt - pcnt);
        }
    }
    printf("%d\n", s);

    free(a);
    return 0;
}
