#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int hh;
    int mm;
} time;

void getTime(time *t) {
    scanf("%d:%d", &t->hh, &t->mm);
}

int diffTime(time *a, time *b) {
    return (a->hh - b->hh) * 60 + (a->mm - b->mm);
}

int main() {
    int T;
    scanf("%d", &T);
    for (; T != 0; T--) {
        int n, thresh;
        scanf("%d%d", &n, &thresh);
        thresh *= 60;
        int i, s;
        for (i = 0, s = 0; i != n; i++) {
            time a, b;
            getTime(&a);
            getTime(&b);
            s += diffTime(&b, &a);
        }
        printf("%s\n", s >= thresh ? "YES" : "NO");
    }
    return 0;
}
