#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/time.h>
#define TIC {struct timeval start, stop; gettimeofday(&start, NULL);
#define TOC gettimeofday(&stop, NULL); printf("%lu sec %luusec\n", stop.tv_sec - \
                                              start.tv_sec, stop.tv_usec -       \
                                              start.tv_usec); }

#define BUFFER_SIZE (1920 * 1088 * 3 / 2)
#define BLK_SIZE  4096

size_t rawread(int fd, char *buffer) {
    size_t bytes_read, total_bytes = 0;
    int i = 0;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE))) {
        if (!bytes_read) {
            break;
        }
        i++;
        total_bytes += bytes_read;
    }
    printf("load %x * %dtimes\n", BUFFER_SIZE, i);
    return total_bytes;
}

size_t slowread(int fd, char *buffer) {
    printf("call func %s\n", __FUNCTION__);
    posix_fadvise(fd, 0, 0, POSIX_FADV_RANDOM);
    return rawread(fd, buffer);
}

size_t fastread(int fd, char *buffer) {
    printf("call func %s\n", __FUNCTION__);
    if (fd <= 0) {
        return fd;
    }
    posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE);
    return rawread(fd, buffer);
}

int main(int argc, char **argv) {
    char *buffer;
    size_t l;
    buffer = malloc(BUFFER_SIZE + 1);
    int fd = open(argv[1], O_RDONLY);

    printf("start read\n");
    TIC
        // not find difference on performance
        // l = fastread(fd, buffer);
        l = slowread(fd, buffer);
    TOC
    printf("read %lu\n", l);

    free(buffer);
    return 0;
}
