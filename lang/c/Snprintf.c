#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
//typedef signed char CHAR;
#define LEN     16
typedef struct sct {
    char s[LEN];
} sct_t;
int main(int argc, char *argv[])
{
    sct_t* p = malloc(sizeof(sct_t));
    if (p) {
        snprintf(p->s, LEN, "EventQ/%d", 15);
        puts(p->s);
        free(p);
    }
    return 0;
}
