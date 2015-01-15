#include "stdio.h"
typedef struct tt {
    union {
        unsigned int val0;
        struct {
            unsigned int v00: 8;
            unsigned int v01: 24;
        };
    };
    union {
        unsigned int val10;
        struct {
            unsigned int v10: 4;
            unsigned int v11: 28;
        };
    };
} tt;

int main()
{
    tt t;
    t.val0 = 0;
    t.v00 = 1;
    printf("0x%08x\n", t.val0);
    t.val0 = 0;
    t.v01 = 1;
    printf("0x%08x\n", t.val0);
    t.val0 = 1;
    printf("0x%08x\n", t.val0);
    return 0;
}
