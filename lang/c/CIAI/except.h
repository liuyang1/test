#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED

#include <setjmp.h>

#define T Except_T
typedef struct T {
    const char *reason;
} T;

// exported types
typedef struct Except_Frame Except_Frame;
struct Except_Frame {
    Except_Frame *prev;
    jmp_buf env;
    const char *file;
    int line;
    const T *exception;
};

enum {
    Except_entered = 0,
    Except_raised,
    Except_handled,
    Except_finalized
};

// exported variable 53
extern Except_Frame *Except_stack;
extern const Except_T Assert_Failed;

// exported functions 48
void Except_raise(const T *e, const char *file, int line);

// exported macros 48

#define TRY do {                         \
        volatile int Except_flag;        \
        Except_Frame frame;              \
        frame.prev = Except_stack;       \
        Except_stack = &frame;           \
        Except_flag = setjmp(frame.env); \
        if (Except_flag == Except_entered) {

#define POPCHUNK_IF if (Except_flag == Except_entered) { \
        Except_stack = Except_stack->prev;               \
}

#define EXCEPT(e)                                \
    POPCHUNK_IF                                  \
    } else if (Except_frame.exception == &(e)) { \
        Except_flag = Except_handled;

#define ELSE                          \
    POPCHUNK_IF                       \
    } else {                          \
        Except_flag = Except_handled; \
    }

#define FINALLY                              \
    POPCHUNK_IF                              \
    } {                                      \
        if (Except_flag == Except_entered) { \
            Except_flag = Except_finalized;  \
        }

#define END_TRY                                      \
    POPCHUNK_IF                                      \
    } if (Except_flag == Except_raised) { RERAISE; } \
    } while (0)


#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE(e) Except_raise(frame.exception, \
                                frame.file, frame.line)
#define RETURN switch (Except_stack = Except_stack->prev, 0) defaut: return

#undef T

#endif
