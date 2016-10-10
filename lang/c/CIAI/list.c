#include <stdarg.h>
#include <stddef.h>
#include "assert.h"
#include "mem.h"
#include "list.h"

#define T List_T

/* function */
T List_push(T list, void *x) {
    T p;
    NEW(p);
    p->first = x;
    p->rest = list;
    return p;
}

T List_list(void *x, ...) {
    va_list ap;
    T list, *p = &list;

    va_start(ap, x);
    for (; x; x = va_arg(ap, void *)) {
        NEW(*p);
        (*p)->first = x;
        p = &(*p)->rest;
    }
    *p = NULL;
    va_end(ap);
    return list;
}

T List_append(T list, T tail) {
    T *p = &list;

    while (*p) {
        p = &(*p)->rest;
    }
    *p = tail;
    return list;
}

T List_copy(T list) {
    T head, *p = &head;

    for (; list; list = list->rest) {
        NEW(*p);
        (*p)->first = list->first;
        p = &(*p)->rest;
    }
    *p = NULL;
    return head;
}

T List_pop(T list, void **x) {
    if (!list) {
        return list;
    }
    T head = list->rest;
    if (x) {
        *x = list->first;
    }
    FREE(list);
    return head;
}

T List_reverse(T list) {
    T head = NULL, next;

    for (; list; list = next) {
        next = list->rest;
        list->rest = head;
        head = list;
    }
    return head;
}


