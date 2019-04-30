#include <stdio.h>

// this could move to one header file, then dynamic include it
#undef LOG
#ifdef INST
#define LOG(s, ...)     printf("inst=%#x " s, INST, ##__VA_ARGS__)
#else
#define LOG(s, ...)     printf(s, ##__VA_ARGS__)
#endif

int main(void)
{
    int inst0 = 0xdead;
    int inst1 = 0xbeef;
    // not defintion of INST, so only output simple log
    LOG("test, %s\n", "no inst");
#undef  INST
#define INST            inst0

#undef LOG
#ifdef INST
#define LOG(s, ...)     printf("inst=%#x " s, INST, ##__VA_ARGS__)
#else
#define LOG(s, ...)     printf(s, ##__VA_ARGS__)
#endif

    // definition of INST, will output inst
    LOG("test, %s\n", "world");

#undef  INST
#define INST            inst1
    // redefinition of INST, will output inst1 as inst
    LOG("test, %s\n", "again");
    return 0;
}
