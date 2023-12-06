#include <stdio.h>

#include <sys/types.h>
#include <fcntl.h>

#define SHOW(x) printf("%-16s: 0x%08x\n", #x, x)

int mask_lst[] = {
    O_WRONLY,
};

int mask(x) {
    // TODO:

}

int main() {
    SHOW(O_APPEND);
    SHOW(O_ASYNC);
    SHOW(O_CLOEXEC);
    SHOW(O_CREAT);
    SHOW(O_DIRECTORY);
    SHOW(O_EXCL);
    SHOW(O_NONBLOCK);
    SHOW(O_NDELAY);
    SHOW(O_SYNC);
    SHOW(O_TRUNC);
    SHOW(O_WRONLY);
    // SHOW(O_PATH);
    // SHOW(O_LARGEFILE);
    // SHOW(O_NOATIME); // since 2.6.8
    // SHOW(O_DIRECT); // since 2.4.10
    return 0;
}

