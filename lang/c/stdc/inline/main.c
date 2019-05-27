#include <stdio.h>

#include "func.h"

int main() {
    printf("%d\n", inline_func());
    /**
     * static inline
     * it's always static, that means it's only visiable for current object
     * it maybe inline
     *
     * We have to have this function, if you use it but don't inline it, then
     * at least make a static version of it in this compilation unit.
     */
    printf("%d\n", static_inline_func());
    /**
     * extern inline
     * I have two functions, one for extern version; one for inline version
     *
     * When it try to call extern version, and there is no extern version, it
     * will throw "undefined reference" error, even there is extern inline
     * version.
     */
    printf("%d\n", extern_inline_func());

    /**
     * Conclusion:
     * I think, in general, we should use static inline version
     * And if we need function pointer, then don't inline it.
     */
    return 0;
}
