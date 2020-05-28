#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// #define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#define DEBUG(...)

typedef struct {
    int **m;
    int *a;
    int n;
} graph;

bool is_adj(int x, int y) {
    return x % 2 == y % 2 || x - y == 1 || y - x == 1;
}

graph build_graph(int *a, int n) {
    assert(n >= 2);
    int **m = malloc(sizeof(int *) * n);
    int b, e;
    for (b = 0; b != n; b++) {
        m[b] = malloc(sizeof(int) * n);
        for (e = 0; e != n; e++) {
            m[b][e] = 0;
        }
    }
    for (b = 0; b != n - 1; b++) {
        for (e = b + 1; e != n; e++) {
            if (is_adj(a[b], a[e])) {
                m[b][e] = m[e][b] = 1;
            }
        }
    }
    graph g = {.m = m, a = a, .n = n};
    return g;
}

void show_graph(graph g) {
    int i, b, e;
    DEBUG("[");
    for (i = 0; i != g.n; i++) {
        DEBUG("%d%s", g.a[i], i != g.n - 1 ? ", " : "");
    }
    DEBUG("]\n");
    for (b = 0; b != g.n; b++) {
        for (e = 0; e != g.n; e++) {
            DEBUG("%d, ", g.m[b][e]);
        }
        DEBUG("\n");
    }
}

void show_array(int *a, int n, char *label) {
    int i;
    DEBUG("%s: [", label != NULL ? label : "");
    for (i = 0; i != n; i++) {
        DEBUG("%d%s", a[i], i != n - 1 ? ", " : "");
    }
    DEBUG("]\n");
}

bool coloring(graph g, int *color, int i, int c) {
    if (i == g.n) {
        return true;
    }
    if (color[i] != -1) {
        DEBUG("color[%d]=%d is colorful, skip\n", i, color[i]);
        return coloring(g, color, i + 1, c);
    }
    color[i] = c;
    DEBUG("color[%d]=%d\n", i, color[i]);
    int j, last;
    for (j = i + 1, last = -1; j != g.n; j++) {
        if (g.m[i][j] == 1 && color[j] == -1) { // adjent and uncolor
            DEBUG("color[%d]=%d last=%d\n", j, c, last);
            color[j] = c;
            if (last != -1) {
                color[last] = -1;
            }
            last = j;
            bool nc = coloring(g, color, i + 1, c + 1);
            if (nc == true) {
                return nc;
            }
        }
    }
    DEBUG("cannot find pair for color=%d\n", c);
    color[i] = -1;
    return false;
}

bool is_paired(graph g) {
    int i;
    int *color = malloc(sizeof(int) * g.n);
    for (i = 0; i != g.n; i++) {
        color[i] = -1;
    }
    bool r = coloring(g, color, 0, 0);
    show_array(color, g.n, "color");

    free(color);
    return r;
}

void dest_graph(graph g) {
    int i;
    for (i = 0; i != g.n; i++) {
        free(g.m[i]);
    }
    free(g.m);
}

int main() {
#define LEN     50
    int t, n, *a = malloc(sizeof(int) * 50), i;
    for (scanf("%d", &t); t != 0; t--) {
        scanf("%d", &n);
        for (i = 0; i != n; i++) {
            scanf("%d", &a[i]);
        }
        graph g = build_graph(a, n);
        show_graph(g);
        printf("%s\n", is_paired(g) ? "YES" : "NO");
        dest_graph(g);
    }
    free(a);
    return 0;
}
