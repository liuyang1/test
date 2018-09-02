#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int *stat(char *a) {
#define LEN     10
    int i, *p = malloc(sizeof(int) * LEN);
    for (i = 0; i != LEN; i++) {
        p[i] = 0;
    }
    for (; *a != '\0'; a++) {
        if (isdigit(*a)) {
            p[*a - '0']++;
        }
    }
    return p;
}

int modsum(char *a, char *b) {
    int *p = stat(a);
    int *q = stat(b);
    int i, j, sum;
    for (i = 1, sum = 0; i != LEN; i++) {
        for (j = 1; j != LEN; j++) {
            sum += p[i] * q[j] * (i % j);
        }
    }
    free(p);
    free(q);
    return sum;
}

int main() {
#define RAWLEN      (10000 + 20)
    char *a = malloc(sizeof(char) * RAWLEN);
    char *b = malloc(sizeof(char) * RAWLEN);
    int casenum, i;
    scanf("%d", &casenum);
    for (i = 0; i != casenum; i++) {
        scanf("%s%s", a, b); // to support whitespace or newline as delimeter.
        printf("%d\n", modsum(a, b));
    }
    free(a);
    free(b);
    return 0;
}
