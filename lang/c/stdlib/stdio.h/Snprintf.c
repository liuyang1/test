#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#define LEN     16
typedef struct sct {
    char s[LEN];
} sct_t;
int main(int argc, char *argv[])
{
    sct_t *p = malloc(sizeof(sct_t));
    if (!p) {
        perror("cannot allocate memory\n");
        return -1;
    }
    int n = snprintf(p->s, LEN, "EventQ/%d", 15);
    printf("return n=%d\n", n);
    if (n > LEN) {
        printf("truncated output\n");
    }
    puts(p->s);
    free(p);
    return 0;
}
