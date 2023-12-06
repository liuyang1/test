#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    const char *fn = "foo";
    // ENXIO when open write only without reader
    int fd = open(fn, O_RDWR);
    if (fd < 0) {
        printf("fail to open fifo to read: fd=%d ",
               fd);
        printf("%d/%s\n", errno, strerror(errno));
        unlink(fn);
        return -1;
    }
    printf("open fifo fn=%s\n", fn);

    size_t sz = 10 * 96 * 4 * 4; // 96k, 4ch 32bit 10ms data
    char *buf = malloc(sz);
    size_t i, n = 10 * 100; // 10seconds, 100buffer
    for (i = 0; i != n; i++) {
        // printf("try to read %zu\n", i);
        ssize_t rd;
        do {
            rd = read(fd, buf, sz);
            printf("rd=%zd\n", rd);
            if (rd == -1) {
                printf("error=%d/%s\n", errno, strerror(errno));
            }
        } while (0 /*rd == -1*/);
        if (rd != -1) {
            printf("buf=%p %zu %s\n", buf, sz, buf);
        }
        usleep(10 * 1000);
    }

    free(buf);
    close(fd);
    return 0;
}

