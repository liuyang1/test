#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[10];
    int ac;
    int time;
} score;

int cmp(score *a, score *b) {
    if (a->ac != b->ac) {
        return a->ac - b->ac;
    }
    if (a->time != b->time) {
        return -1 * (a->time - b->time);
    }
    return -1 * strcmp(a->name, b->name);
}

static score m;
void put(score *p) {
    if (cmp(p, &m) > 0) {
        m = *p;
    }
}

void show() {
    printf("%s\n", m.name);
}

void reset() {
    m.ac = -1;
}

int main() {
    int casenum, i;
    scanf("%d", &casenum);
    for (i = 0; i != casenum; i++) {
        int num, j;
        scanf("%d", &num);
        reset();
        score t;
        for (j = 0; j != num; j++) {
            scanf("%s%d%d", t.name, &t.ac, &t.time);
            put(&t);
        }
        show();
    }
}
