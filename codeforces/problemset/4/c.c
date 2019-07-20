#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int h;
    char *s;
    unsigned int counter;
} Entry;

#define     LEN     200003 // prime after 100000 * 2
Entry a[LEN];

unsigned int hash(char *s) {
    unsigned h = 0;
    for (; *s != '\n' && *s != '\0'; s++) {
        h = *s * 97 + h;
    }
    return h;
}

int main() {
    memset(a, 0x0, sizeof(Entry) * LEN);
    int T;
    scanf("%d", &T);
    char s[40];
    unsigned int i;
    while (T--) {
        scanf("%s", s);
        unsigned int h = hash(s);
        unsigned int idx = h % LEN;
        // linear order to extend when collision
        for (i = idx; ; i = (i + 1) % LEN) {
            if (a[i].s == NULL) {
                a[i].s = strdup(s);
                a[i].h = h;
                a[i].counter = 1;
                printf("OK\n");
                break;
            }
            if (a[i].h == h && strcmp(a[i].s, s) == 0) {
                printf("%s%d\n", s, a[i].counter);
                a[i].counter++;
                break;
            }
        }
    }
    for (i = 0; i != LEN; i++) {
        if (a[i].s != NULL) {
            free(a[i].s);
        }
    }
    return 0;
}
