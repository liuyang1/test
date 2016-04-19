#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    FILE* fp = fopen("test", "a+");

    const char s[] = "hello, world!\n";
    fwrite(s, strlen(s), sizeof(char), fp);

    // call fsync to synchronize file to storage
    int fd = fileno(fp);
    fsync(fd);

    fclose(fp);
    return 0;
}
