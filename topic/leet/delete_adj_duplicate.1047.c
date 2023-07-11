#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct Stack {
    char *v;
    size_t len;
    size_t cap;
};

struct Stack *ctor(size_t cap) {
    struct Stack *p = malloc(sizeof(struct Stack));
    p->v = malloc(sizeof(char) * cap);
    p->cap = cap;
    p->len = 0;
    return p;
}

void dtor(struct Stack *p) {
    free(p->v);
    free(p);
}

void push(struct Stack *p, char x) {
    p->v[p->len++] = x;
    assert(p->len != p->cap); // it will overwrite when next push
}

int pop(struct Stack *p) {
    if (p->len == 0) {
        return -1;
    }
    return p->v[--p->len];
}

int peek(struct Stack *p) {
    if (p->len == 0) {
        return -1;
    }
    return p->v[p->len - 1];
}

char *dump(struct Stack *p) {
    p->v[p->len] = '\0';
    return strdup(p->v);
}

char *delete_adj_duplicate(char *s) {
    struct Stack *p = ctor(1024);
    for (; *s != '\0'; s++) {
        if (*s == peek(p)) {
            pop(p);
        } else {
            push(p, *s);
        }
    }
    char *r = dump(p);
    dtor(p);
    return r;
}

int main() {
    char s[] = "abbahellehoaca";
    char *r = delete_adj_duplicate(s);
    printf("return %s\n", r);
    free(r);
    return 0;
}
