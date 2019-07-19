#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/** s, t, p
 * erase letter from P, insert it to S (anywhere)
 * if operate the string to t
 *
 * s="ab" t="acxb" p="cax"
 * => s="acb" p="ax"
 * => s="acxb" p="a" => s == t
 *
 * s="a" t="b" p="b"
 * => fail to operate
 *
 * s="a" t="aaaa" p="aabbcc"
 * => fail to operate
 */

// OJ have no ssize_t
int my_getline(char **lineptr, size_t *n, FILE *stream) {
    const int size = 200;
    if (*lineptr == NULL) {
        *lineptr = malloc(size);
        *n = size;
    }
    fgets(*lineptr, size, stream);
    return strlen(*lineptr);
}

char *minus(char *t, char *s) {
    char *d = strdup(t);
    uint32_t i = 0;
    for (; *t != '\0'; t++) {
        if (*s == *t) {
            s++;
            continue; // skip same letter from s
        }
        d[i++] = *t;
    }
    if (*s != '\0') { // s have different letter
        free(d);
        return NULL;
    }
    d[i] = '\0';
    return d;
}

uint32_t *counter(char *s) {
    uint32_t *c = malloc(sizeof(uint32_t) * 26);
    memset(c, 0, sizeof(uint32_t) * 26);
    for (; *s; s++) {
        assert(*s >= 'a');
        assert(*s <= 'z');
        c[*s - 'a']++;
    }
    return c;
}

uint32_t greater(uint32_t *a, uint32_t *b) {
    uint32_t i;
    for (i = 0; i != 26; i++) {
        if (a[i] < b[i]) {
            return 0;
        }
    }
    return 1;
}

uint32_t sln(char *s, char *t, char *p) {
    char *d = minus(t, s);
    if (d == NULL) {
        return 0;
    }
    uint32_t *a = counter(d);
    uint32_t *b = counter(p);
    uint32_t r = greater(b, a);
    free(d);
    free(a);
    free(b);
    return r;
}

int main() {
    uint32_t T;
    scanf("%u", &T);
    char *s = NULL, *t = NULL, *p = NULL;
    size_t a = 0, b = 0, c = 0;
    getchar();
    while (T--) {
        size_t l = my_getline(&s, &a, stdin);
        s[l - 1] = '\0';
        l = my_getline(&t, &b, stdin);
        t[l - 1] = '\0';
        l = my_getline(&p, &c, stdin);
        p[l - 1] = '\0';
        int r = sln(s, t, p);
        printf("%s\n", r ? "YES" : "NO");
    }
    realloc(s, 0);
    realloc(t, 0);
    realloc(p, 0);
    return 0;
}
