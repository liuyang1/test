#include <stdio.h>
#include <errno.h>

#define SHOW(x) printf("%-16s: %d\n", #x, x)
int main() {
    SHOW(EBADF);
    SHOW(EFAULT);
    SHOW(ERANGE);
    SHOW(EDOM);
    SHOW(EILSEQ); // illegal sequence
    SHOW(ENOSPC); // no space
    SHOW(EAGAIN); // for async IO
    // there is no data available right now, try again later
    SHOW(EWOULDBLOCK);

    return 0;
}
