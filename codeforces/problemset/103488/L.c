/** https://codeforces.com/gym/103488/problem/L */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    char *s = malloc(n + 1);
    scanf("%s", s);
    char *t = malloc(m + 1);

    memcpy(t, s, strlen(s) + 1);
    t[m] = '\0';
    m--;
    for (; m != 0 && m >= n; m--) {
        t[m] = 'z';
    }
    for (; m >= 0; m--) {
        if (t[m] == 'a') {
            t[m] = 'z';
        } else {
            t[m] = t[m] - 1;
            break;
        }
    }
    printf("%s\n", t);

    free(s);
    free(t);
}

