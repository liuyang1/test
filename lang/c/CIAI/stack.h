#ifndef STACK_INCLUDED
#define STACK_INCLUDED

// define this to make interface looks pretty and more readable.
#define T Stack_T

// XXX: so T is stuct T * type, remind it's pointer type
typedef struct T *T;

extern T Stack_new(void);
extern int Stack_empty(T stk);
extern void Stack_push(T stk, void *x);
extern void *Stack_pop(T stk);
// pass a pointer to T, so we could reset it to NULL to ease dangling pointer
// issue.
// >>> It helps, but not so useful.
extern void Stack_free(T *stk);

#undef T

#endif
