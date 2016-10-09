#include <stdlib.h>
#include <stddef.h>
#include "assert.h"
#include "except.h"
#include "mem.h"

const Except_T Mem_Failed = { "Allocation Failed"};
#define RAISE_MEM_FAILED \
        if (file == NULL) { \
            RAISE(Mem_Failed); \
        } else { \
            Except_raise(&Mem_Failed, file, line); \
        }

void *Mem_alloc(long nbytes, const char *file, int line) {
    void *ptr;

    assert(nbytes > 0);
    ptr = malloc(nbytes);
    if (ptr == NULL) {
        RAISE_MEM_FAILED
    }
    return ptr;
}

void Mem_free(void *ptr, const char *file, int line) {
    if (ptr) {
        free(ptr);
    }
}

void *Mem_resize(void *ptr, long nbytes, const char *file, int line) {
    assert(ptr);
    assert(nbytes > 0);
    ptr = realloc(ptr, nbytes);
    if (ptr == NULL) {
        RAISE_MEM_FAILED
    }
    return ptr;
}


