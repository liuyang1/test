#include "stdio.h"
#include "stdarg.h"


////////////////////////////////////////////////////////////////////////////////
// output log with callee level
// better choose 2 x whitepsace as prefix unit.
// setting _loghirh to negative value, to set start log callee level output.
// using -2, I think its better choosen.
// when callee level complexer, this value should setting more less.
int _loghirh = -1;
void debugStr(const char *s0, char *s1)
{
    int i;
    for(i = 0; i < _loghirh; i++) {
        printf("  ");
    }
    printf("%s %s\n", s0, s1);
}
#define INFN        {_loghirh++;debugStr(__FUNCTION__," in  {");}
#define OUTFN       {debugStr(__FUNCTION__," out }");_loghirh--;}
////////////////////////////////////////////////////////////////////////////////

int fib(int n)
{
    INFN;
    int v;
    if (n == 0 || n == 1) {
        v = 1;
    } else {
        v = fib(n - 1) + fib(n - 2);
    }
    debugStr("test output", NULL);
    OUTFN;
    return v;
}

int main(int argc, char *argv[])
{
    fib(3);
    return 0;
}
