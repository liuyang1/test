#include <stdio.h>
#include <errno.h>

#define SHOW(x) printf("%s: %d\n", #x, x)
int main() {
    SHOW(EBADF);
    SHOW(EFAULT);
    return 0;
}
