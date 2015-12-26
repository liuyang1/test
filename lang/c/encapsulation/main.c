#include <stdio.h>
#include "queue.h"

int main()
{
    queue_t *q = openqueue();

    char s[] = "abc";
    enqueue(q, s);
    char *p = dequeue(q);
    printf("%s\n", p);

    closequeue(q);
    return 0;
}
