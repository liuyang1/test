/**
 * Cheating this WARN_ONCE idea from Linux kernel code
 * https://elixir.bootlin.com/linux/v4.9.156/source/include/asm-generic/bug.h#L126
 * 
 * With this macro, we could add dynamic checking logic in formal code,
 * and it only warning ONE TIME.
 */

#include <stdbool.h>
#include <stdio.h>

#define WARN_ONCE(cond, fmt ...) ({                     \
        static bool warned = false;                     \
        int ret_warn_once = !!(cond);                   \
        if (ret_warn_once == true && warned == false) { \
            warned = true;                              \
            printf(fmt);                                \
        }                                               \
    })

int func(int cond) {
    WARN_ONCE(cond, "cond=%d have issue\n", cond);
    return cond;
}

int main() {
    func(0); // don't show warning msg, as cond is false
    func(1); // show warning msg, as cond is true & first time
    func(2); // don't show warning msg, as it already showed up
    return 0;
}
