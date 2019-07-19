#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t sumRow(char *p, uint32_t m, uint32_t n, uint32_t i) {
    uint32_t j, s;
    for (j = s = 0; j != n; j++) {
        s += p[i * n + j] == '*';
    }
    return s;
}

uint32_t sumCol(char *p, uint32_t m, uint32_t n, uint32_t i) {
    uint32_t j, s;
    for (j = s = 0; j != m; j++) {
        s += p[j * n + i] == '*';
    }
    return s;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/** time exceed 2s limit
 * O(m*n*n)
 * add table to store temp result
 */
uint32_t sln(char *p, uint32_t m, uint32_t n) {
    uint32_t i, j, max, t;
    uint32_t *row = malloc(sizeof(uint32_t) * m);
    uint32_t *col = malloc(sizeof(uint32_t) * n);
    for (i = 0; i != m; i++) {
        row[i] = sumRow(p, m, n, i);
    }
    for (j = 0; j != n; j++) {
        col[j] = sumCol(p, m, n, j);
    }
    for (i = max = 0; i != m; i++) {
        for (j = 0; j != n; j++) {
            t = row[i] + col[j] - (p[i * n + j] == '*');
            max = MAX(max, t);
        }
    }
    free(row);
    free(col);
    return (m + n - 1 - max);
}

void show(char *p, uint32_t m, uint32_t n) {
    uint32_t i, j;
    for (i = 0; i != m; i++) {
        for (j = 0; j != n; j++) {
            printf("%c", p[i * n + j]);
        }
        printf("\n");
    }
}
int main() {
    uint32_t T;
    scanf("%u", &T);
    while (T--) {
        uint32_t m, n, i, len, j;
        scanf("%u%u", &m, &n);
        len = m * n;
        char *t = malloc(sizeof(char) * len);
        getchar();
        for (i = 0; i != len; i++) {
            do {
                scanf("%c", t + i);
            } while (t[i] == '\n');
        }
#if 0
        printf("%u %u\n", m, n);
        show(t, m, n);
        continue;
#endif
        uint32_t r = sln(t, m, n);
        printf("%d\n", r);
        free(t);
    }
    return 0;
}
