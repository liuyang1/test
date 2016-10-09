#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "except.h"

#define T Except_T

Except_Frame *Except_stack = NULL;

void Except_raise(const T *e, const char *file, int line) {
    Except_Frame *p = Except_stack;
    assert(e);
    if (p == NULL) {
        fprintf(stderr, "Uncaught except");
        if (e->reason) {
            fprintf(stderr, " %s", e->reason);
        } else {
            fprintf(stderr, " at 0x%p", e);
        }
        if (file && line > 0) {
            fprintf(stderr, " raise at %s:%d\n", file, line);
        }
        fprintf(stderr, "aborting...\n");
        fflush(stderr);
        abort();
    }
    p->exception = e;
    p->file = file;
    p->line = line;

    longjmp(p->env, Except_raised);
}

