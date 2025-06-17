#include <stdio.h>

// declare warning in compiler message
#warning        this message
#error          that message

#define MY_MACRO 42

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define PRINT_MACRO(macro) #macro "=" TOSTRING(macro)

#pragma message(PRINT_MACRO(MY_MACRO))
#pragma message(PRINT_MACRO(UNDEFINED))
int main()
{
    return 0;
}
