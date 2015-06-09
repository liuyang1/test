#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

typedef struct {
}Empty;

int main()
{
    // malloc of ZERO size, may return NULL
    // but in this test, return Non-NULL
    Empty *p = malloc(sizeof(Empty));
    printf("Empty pointer %p size %zd\n", p, sizeof(Empty));
    Empty em;
    bzero(&em, sizeof(em));
    printf("Empty struct %p size %zd\n", &em, sizeof(em));
    free(p);
}
