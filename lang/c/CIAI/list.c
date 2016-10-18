#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
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

void *List_car(T list) {
    return list->first;
}

T List_cdr(T list) {
    return list->rest;
}

T List_cons(void *x, T list) {
    return List_push(list, x);
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

int List_length(T list) {
    int n;
    for (n = 0; list; list = List_cdr(list)) {
        n++;
    }
    return n;
}

void List_free(T *list) {
    T next;
    assert(list);
    for (; *list; *list = next) {
        next = (*list)->rest;
        FREE(*list);
    }
}

void List_map(T list, void apply(void **x, void *cl), void *cl) {
    assert(apply);
    for (; list; list = list->rest) {
        apply(&list->first, cl);
    }
}

void **List_toArray(T list, void *end) {
    int i, n = List_length(list);
    void **array = ALLOC((n + 1) * sizeof(*array));
    for (i = 0; i < n; i++) {
        array[i] = list->first;
        list = list->rest;
    }
    array[i] = end;
    return array;
}

void List_show_in(T list) {
    bool start = true;
    char *fmt;
    printf("(");
    while (list != NULL) {
        if (start) {
            fmt = "%p";
            start = false;
        } else {
            fmt = " %p";
        }
        printf(fmt, List_car(list));
        list = List_cdr(list);
    }
    printf(")\n");
}
