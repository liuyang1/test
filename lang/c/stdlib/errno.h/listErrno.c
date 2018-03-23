#include <stdio.h>
#include <errno.h>

#define SHOW(x) printf("%-10s: %d\n", #x, x)
int main() {
    SHOW(EBADF);
    SHOW(EFAULT);
    SHOW(ERANGE);
    SHOW(EDOM);
    SHOW(EILSEQ); // illegal sequence
    return 0;
}
