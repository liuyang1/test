#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "abp.h"

typedef struct abp_st {
    unsigned int num;
    unsigned int size;
    void **bufLst;
    bool *bufStatusLst;
    unsigned int bufLstNum;
    pthread_t thread;
} ABP;

typedef struct abp_st ABP;

// internal funciton declaration
void *allocRoutineI(void *arg);

ABP *abp_create(int num, int size) {
    ABP *p = malloc(sizeof(ABP));
    if (p == NULL) {
        goto error0;
    }
    p->num = num;
    p->size = size;
    p->bufLst = malloc(sizeof(void *) * num);
    if (p->bufLst == NULL) {
        goto error1;
    }
    p->bufStatusLst = malloc(sizeof(bool) * num);
    if (p->bufStatusLst == NULL) {
        goto error2;
    }
    p->bufLstNum = 0;
    int ret = pthread_create(&p->thread, NULL, allocRoutineI, p);
    if (ret != 0) {
        goto error3;
    }
    return p;
    void *retptr;
error4:
    ret = pthread_join(p->thread, &retptr);
error3:
    free(p->bufStatusLst);
error2:
    free(p->bufLst);
error1:
    free(p);
error0:
    return NULL;
}

void abp_labelDestroy(ABP *p) {
    assert(p != NULL);

}

void abp_waitDestory(ABP *p) {
    assert(p != NULL);
}

// internal function
void *allocRoutineI(void *arg) {
    assert(arg != NULL);
    ABP *p = arg;
    printf("ABP=%p\n", p);
    return NULL;
}
