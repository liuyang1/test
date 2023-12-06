#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    static const char *fn = "foo1";
    mode_t mode = 0666;
    int r = mkfifo(fn, mode);
    if (r != 0) {
        printf("fail to mkfifo, r=%d\n", r);
    }
    printf("mkfifo fn=%s\n", fn);
    fn = "foo2";
    r = mkfifo(fn, mode);
    if (r != 0) {
        printf("fail to mkfifo, r=%d\n", r);
    }

    // ENXIO when open write only without reader
    int fd = open(fn, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        printf("fail to open fifo to write: fd=%d ",
               fd);
        printf("%d/%s\n", errno, strerror(errno));
        unlink(fn);
        return -1;
    }
    printf("open fifo fn=%s\n", fn);

    size_t sz = 10 * 96 * 4 * 4; // 96k, 4ch 32bit 10ms data
    void *buf = malloc(sz);
    size_t i, n = 10 * 100; // 10seconds, 100buffer
    for (i = 0; i != n; i++) {
        printf("try to write %zu\n", i);
        snprintf(buf, sz, "test msg %zu", i);
        ssize_t wr = write(fd, buf, sz);
        printf("wr=%zd\n", wr);
        if (wr == -1) {
            printf("error=%d/%s\n", errno, strerror(errno));
        }
        usleep(10 * 1000);
    }
    free(buf);
    close(fd);
    unlink(fn);
    return 0;
}
