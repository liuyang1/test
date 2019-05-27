#include "func.h"

#if 0
int extern_inline_func() {
    printf("this is extern version of %s\n", __func__);
    return 999;
}
#endif

inline int inline_func() {
    return 4201;
}
