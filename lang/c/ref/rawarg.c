#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRING_RAW_MAX_LEN 128
typedef struct {
    int32_t len;
    int8_t raw[STRING_RAW_MAX_LEN];
} arg_st;

#define DELIM     ' '

void showraw(arg_st *p) {
    printf("arg_st=%p len=%d raw='%s'\n", p, p->len, p->raw);
}

int arg2raw(arg_st *p, int argc, char **argv) {
    int i, len, cap;
    for (i = 0, len = 0, cap = STRING_RAW_MAX_LEN; i != argc; i++) {
        len += snprintf(p->raw + len, cap - len, "%s%c", argv[i], DELIM);
    }
    p->raw[len - 1] = '\0'; // change final whitespace to \0
    p->len = len;
    return 0;
}

int raw2arg(arg_st *p, int *o_argc, char ***o_argv) {
    char *r = p->raw, *o = p->raw;
    char *e;
    int c = 0;
    int m = sizeof(char *);
    char **v = NULL;
    do {
        e = strchr(r, DELIM);
        c++;
        v = realloc(v, m * c);
        v[c - 1] = strndup(r, e - r);
        r = e + 1; // pass the delim
    } while (e != NULL);
    *o_argc = c;
    *o_argv = v;
    return 0;
}

int freearg(int argc, char **argv) {
    int i;
    for (i = 0; i != argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void showarg(int argc, char **argv) {
    int i;
    for (i = 0; i != argc; i++) {
        printf("%d:%s\n", i, argv[i]);
    }
}

int main() {
    char *argv[] = { "hello", "world", "dumb", "test"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    arg_st arg;
    arg2raw(&arg, argc, argv);
    showraw(&arg);

    int c;
    char **v;
    raw2arg(&arg, &c, &v);
    showarg(c, v);
    freearg(c, v);
    return 0;
}
