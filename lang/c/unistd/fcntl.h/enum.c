#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
#define show(x) printf("%s:\t%03d 0x%03x\n", #x, x, x)
    show(O_CREAT);
    show(O_RDONLY);
    show(O_WRONLY);
    show(O_TRUNC);
    return 0;
}
