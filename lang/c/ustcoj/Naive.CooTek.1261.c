#include <stdio.h>
#include <stdlib.h>

// Too Naive Solution. Need improve performance.
int len = 1000 * 1000;
long long *tbl = NULL;

void init() {
    tbl = malloc(sizeof(*tbl) * len);
}

void deinit() {
    free(tbl);
}

void produce(int id, int num) {
    tbl[id] += num;
}

void consume(int id, int num) {
    if (tbl[id] >= num) {
        tbl[id] -= num;
    } else {
        printf("WARNING: consume %d > %lld for item=%d\n", num, tbl[id], id);
        tbl[id] = 0;
    }
}

long long query(int begin, int end) {
    int i;
    long long sum;
    for (i = begin, sum = 0; i <= end; i++) {
        sum += tbl[i];
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
