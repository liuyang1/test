#include "stdio.h"
typedef enum{
    A = 0,
    B,
    C
} ET;

#define ENUMCASE2STR(x)         case(x): return #x;
char* enum2str(ET e)
{
    switch(e){
        ENUMCASE2STR(A);
        ENUMCASE2STR(B);
        default: return "nil";
    }
}
int main(int argc, char *argv[])
{
    printf("%s\n", enum2str(A));
    return 0;
}
