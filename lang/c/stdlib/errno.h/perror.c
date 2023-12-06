#include <errno.h>
#include <stdio.h>

int main() {
    errno = 42;
    perror("test msg");
    return 0;
}
