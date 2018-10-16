#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/uio.h>


int main() {
    char sa[][20] = {"foo", "bar", "baz", "灰化肥发黑","黑化肥发灰"};
    static const size_t n = sizeof(sa) / sizeof(sa[0]);
    size_t *na = malloc(sizeof(size_t) * n);
    size_t i, j, nwritten;
    for (i = 0; i != n; i++) {
        na[i] = strlen(sa[i]);
    }

    struct iovec *piov = malloc(sizeof(struct iovec) * n);

    for (j = 0; j != n; j++) {
        for (i = 0; i != n; i++) {
            size_t c = (i + j) % n;
            piov[i].iov_base = sa[c];
            piov[i].iov_len = na[c];
        }

        printf("shift with j=%lu:\n", j);
        nwritten = writev(STDOUT_FILENO, piov, n);
        printf("\n");
    }

    free(na);
    free(piov);
    return 0;

}
