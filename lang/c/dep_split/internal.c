#include <stdio.h>

#include "export.h"
#include "internal.h"

// From wiki page
// https://en.wikipedia.org/wiki/Assertion_%28computing%29#Static_assertions
#define COMPILE_TIME_ASSERT(pred) switch(0){case 0:case pred:;}

int func() {
    COMPILE_TIME_ASSERT(MAGICNUM == MAGICNUM_INTERNAL);
    printf("Passing compile confirm MAGICNUM is equal to MAGINCNUM_INTERNAL\n");
    return 0;
}
