#include <stdio.h>
#include <stdlib.h>

long long *tbl = NULL;
int base = 1024 * 1024 - 1;

void init() {
    int len = 1024 * 1024 * 2, i;
    tbl = malloc(sizeof(*tbl) * len);
    for (i = 0; i != len; i++) {
        tbl[i] = 0;
    }
}

void deinit() {
    free(tbl);
}

void produce(int id, int num) {
    int idx = base + id;
    for (; idx != 0; idx = (idx - 1) / 2) {
        tbl[idx] += num;
    }
}

void consume(int id, int num) {
    int idx = base + id;
    if (tbl[idx] < num) {
        printf("WARNING: consume %d > %lld for item=%d\n", num, tbl[id], id);
        num = tbl[idx];
    }
    for (; idx != 0; idx = (idx - 1) / 2) {
        tbl[idx] -= num;
    }
}

long long query(int begin, int end) {
    if (begin != 0) {
        return query(0, end) - query(0, begin - 1);
    }
    if (end < 0) {
        return 0;
    }
    int idx = base + end;
    long long sum = 0;
    while (idx != 0) {
        if (idx % 2 == 1) {
            sum += tbl[idx];
            idx -= 1;
        } else {
            idx = (idx - 1) / 2;
        }
    }
    return sum;
}

int main() {
    init();

    char action;
    int a, b;
    while (scanf("%c %d %d", &action, &a, &b) != EOF) {
        switch (action) {
            case 'p': produce(a, b); break;
            case 'c': consume(a, b); break;
            case 'q': printf("%lld\n", query(a, b)); break;
            default: break;
        }
    }

    deinit();
    return 0;
}
