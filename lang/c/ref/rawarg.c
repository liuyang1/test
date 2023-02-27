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

char *my_strndup(const char*s, size_t n) {
    printf("%p:'%s'\n", s, s);
    char *r = malloc(sizeof(char) * (n + 1));
    strncpy(r, s, n);
    r[n] = '\0';
    return r;
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
        if (e == NULL) {
            v[c - 1] = strdup(r);
        } else {
            v[c - 1] = my_strndup(r, e - r);
        }
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
    printf("argc=%d\n", argc);
    int i;
    for (i = 0; i != argc; i++) {
        printf("%d:'%s'\n", i, argv[i]);
    }
}

int unit(int argc, char **argv) {
    // char *argv[] = { "hello", "world", "dumb", "test"};
    arg_st arg;
    arg2raw(&arg, argc, argv);
    showraw(&arg);

    int c = 0;
    char **v = NULL;
    raw2arg(&arg, &c, &v);
    showarg(c, v);
    freearg(c, v);
    return 0;
}
int main() {
    int argc;
#if 1
    char *argv[] = {"hello"};
    argc = sizeof(argv) / sizeof(argv[0]);
    unit(argc, argv);
#endif

    char *argv1[] = {"pipe", "test"};
    argc = sizeof(argv1) / sizeof(argv1[0]);
    unit(argc, argv1);

#if 1
    char *argv2[] = {"cap", "test"};
    argc = sizeof(argv2) / sizeof(argv2[0]);
    unit(argc, argv2);
#endif

    return 0;
}
