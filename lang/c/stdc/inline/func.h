#ifndef _FUNC_H_
#define _FUNC_H_

#include <stdio.h>

static inline int static_inline_func() {
    return 42;
}

extern inline int extern_inline_func() {
    printf("this is inline version of %s\n", __func__);
    return 666;
}

// multiple define in main.c and func.c
#if 0
inline int inline_func() {
    return 1024;
}
#endif
#endif
