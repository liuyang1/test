#include <stdio.h>

int main() {
    // These two styles are different.
    /**
     * open("testFreopen.c", O_RDONLY)
     * dup3(3, 0, 0)
     * close(3)
     */
#ifdef FREOPEN
    FILE *fp = freopen("testFreopen.c", "r", stdin);
#else
    fclose(stdin);
    FILE *fp = stdin = fopen("testFreopen.c", "r");
    /**
     * close(0);
     * open("testFreopen.c", O_RDONLY)
     * so next `getchar()` function read from 0, so print nothing
     */
#endif
    printf("stdin=%p == fp=%p\n", stdin, fp);


    // WHY?
    // FILE *stream is based on file descriptor at linux platform.
    // FILE *stream point to address of FD.
    // so we could associate another stream with FILE*

    char c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    return 0;
}
