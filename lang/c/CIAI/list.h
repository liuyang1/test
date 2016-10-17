#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#define T List_T
typedef struct T *T;

// Why reveal this struct?
// - can be defined and init statically. It's useful for building list at
// compile time, and avoid allocation.
// - other struct could embedded List
// - a null List_T is an empty list.
struct T {
    // this struct look like list in lisp
    T rest;
    void *first;
};

/**
 * List_T is immutable struct, so remember to use returned T
 */
/** called with N nonnull pointers followed by one null pointer
 * create a list which N nodes
 */
extern T List_list(void *x, ...);

extern T List_append(T list, T tail);
extern T List_copy(T list);

extern T List_pop(T list, void **x);
/** push one element to list, and return NEW list.
 */
extern T List_push(T list, void *x);

extern T List_reverse(T list);
extern int List_length(T list);
extern void List_free(T *list);
extern void List_map(T list, void apply(void **x, void *cl), void *cl);
extern void **List_toArray(T list, void *end);
extern void List_show(T list);
#define List_show(a) {printf("%s: ", #a); List_show(a); }

#undef T
#endif
