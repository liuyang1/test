#include <stdio.h>
#include <unistd.h>

int main() {
    long l = pathconf("/", _PC_PATH_MAX);
    printf("max path=%ld\n", l);
    return 0;
}
