#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int len = 1000 * 1000;
    char *s = malloc(len);
    char *f = malloc(len);
    memset(s, 0x0, len);
    memset(f, 0x0, len);
    fgets(s, len, stdin);
    len = strlen(s);
    s[len--] = '\0';
    // printf("%s", s);
    int b, e, cnt = 0;
    for (b = 0, e = len - 1; b <= e; ) {
        if (s[b] == s[e]) {
            f[b] = f[e] = 1;
            if (b == e) {
                cnt++;
            } else {
                cnt += 2;
            }
            b++;
            e--;
            // printf("%c,", s[b]);
        } else {
            if (s[b] == s[e-1]) {
                f[e] = 0;
                e--;
            } else if (s[b+1] == s[e]) {
                f[b] = 0;
                b++;
            } else {
                f[b] = f[e] = 0;
                b++;
                e--;
            }
        }
    }
    // printf("cnt=%d\n", cnt);
    if (cnt < len / 2) {
        printf("IMPOSSIBLE\n");
    } else {
        for (b = 0; b != len; b++) {
            if (f[b]) {
                printf("%c", s[b]);
            }
        }
        printf("\n");
    }
    free(s);
    free(f);
    return 0;
}
