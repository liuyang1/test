#include <stdio.h>
#include <setjmp.h>

// errno must not zero
#define ERR_DIVZERO 2
// This jmpPoint must be global
jmp_buf jmpPoint;

int div(int a, int b)
{
    if (b == 0) {
        // jump back to "setjmp" point
        longjmp(jmpPoint, ERR_DIVZERO);
    }
    return a / b;
}

void catchExcept(int jmpret)
{
    switch (jmpret) {
        case ERR_DIVZERO:
            printf("catch except=%x div zero\n", jmpret);
            break;
        default:
            printf("catch except=%x\n", jmpret);
    }
}

int main()
{
    // - when setjmp, will save context and register, and return zero.
    // - when call longjmp, it will return back here, and restore context and
    // register, and return non-zero.
    int jmpret = setjmp(jmpPoint);
    // like *TRY*
    if (jmpret == 0) {
        int a, b;
        a = 4;
        b = 2;
        printf("a=%d b=%d a/b=%d\n", a, b, div(a, b));
        b = 0;
        printf("a=%d b=%d a/b=%d\n", a, b, div(a, b));
    } else {
        // like *CATCH*
        // catch non-zero return value.
        catchExcept(jmpret);
        return jmpret;
    }
    return 0;
}
