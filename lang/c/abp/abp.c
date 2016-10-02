#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "abp.h"

#define CHECK_PTR_GOTO(p, label, ...) \
    if (p == NULL) { printf(__VA_ARGS__); goto label; }

// one stuipd record table
typedef enum {
    UNINIT = 0,
    UNUSED,
    INUSE
} RecordStatus;

typedef struct rt_st {
    RecordStatus *aStatus;
    void **aHandle;
    unsigned int numTarget;
    unsigned int numUnused;
    unsigned int numInuse;
    pthread_mutex_t lock;
} RecordTbl;

RecordTbl *rt_create(int num);
void rt_destory(RecordTbl *p);
void rt_load(RecordTbl *p, void *handle);
void *rt_unload(RecordTbl *p);
void *rt_acquire(RecordTbl *p);
void rt_release(RecordTbl *p, void *handle);

RecordTbl *rt_create(int num) {
    RecordTbl *p = malloc(sizeof(RecordTbl));
    if (p == NULL) {
        return NULL;
    }
    p->numTarget = num;
    p->numUnused = p->numInuse = 0;
    p->aStatus = malloc(sizeof(RecordStatus) * num);
    CHECK_PTR_GOTO(p->aStatus, fail, "malloc p->aStatus fail\n");
    int i;
    for (i = 0; i != p->numTarget; i++) {
        p->aStatus[i] = UNINIT;
    }
    p->aHandle = malloc(sizeof(void *) * num);
    CHECK_PTR_GOTO(p->aHandle, fail, "malloc p->aHandle fail\n");
    pthread_mutex_init(&p->lock, NULL);
    return p;
fail:
    rt_destory(p);
    return NULL;
}

void rt_destory(RecordTbl *p) {
    assert(p != NULL);
    assert(p->numInuse == 0);
    assert(p->numUnused == 0);
    if (p->aHandle != NULL) {
        free(p->aHandle);
    }
    if (p->aStatus != NULL) {
        free(p->aStatus);
    }
    // TODO: how to check is lock init or not
    pthread_mutex_destroy(&p->lock);
}

void rt_lock(RecordTbl *p) {
    pthread_mutex_lock(&p->lock);
}

void rt_unlock(RecordTbl *p) {
    pthread_mutex_unlock(&p->lock);
}

/**
 * UNINIT ---load---> UNUSED --acquire--> INUSE
 *        <--unload--        <-release---
 */

size_t firstStatus(RecordTbl *p, RecordStatus st) {
    size_t i;
    for (i = 0; i != p->numTarget; i++) {
        if (p->aStatus[i] == st) {
            break;
        }
    }
    return i;
}

void rt_load(RecordTbl *p, void *handle) {
    assert(p != NULL);
    assert(p->numUnused < p->numTarget);

    rt_lock(p);

    size_t i = firstStatus(p, UNINIT);
    assert(i != p->numTarget);
    p->aStatus[i] = UNUSED;
    p->aHandle[i] = handle;
    p->numUnused++;

    rt_unlock(p);
}

void *rt_unload(RecordTbl *p) {
    assert(p != NULL);

    pthread_mutex_lock(&p->lock);
    size_t i = firstStatus(p, UNUSED);
    assert(i != p->numTarget);
    p->aStatus[i] = UNINIT;
    void *handle = p->aHandle[i];
    p->numUnused--;

    pthread_mutex_unlock(&p->lock);
    return handle;
}

void *rt_acquire(RecordTbl *p) {
    void *ret = NULL;
    assert(p != NULL);
    rt_lock(p);
    size_t i = firstStatus(p, UNUSED);
    if (i == p->numTarget) {
        goto end;
    }
    p->aStatus[i] = INUSE;
    ret = p->aHandle[i];
    p->numUnused--;
    p->numInuse++;
end:
    rt_unlock(p);
    return ret;
}

void rt_release(RecordTbl *p, void *handle) {
    assert(p != NULL);
    rt_lock(p);
    size_t i;
    for (i = 0; i != p->numTarget; i++) {
        if (p->aHandle[i] == handle && p->aStatus[i] == INUSE) {
            break;
        }
    }
    assert(i != p->numTarget);
    p->aStatus[i] = UNUSED;
    p->numUnused++;
    p->numInuse--;
    rt_unlock(p);
}

typedef void *(*AllocFn)(void *);
typedef void (*FreeFn)(void *);
typedef struct abp_st {
    AllocFn allocFn; // function to allocate buffer
    FreeFn freeFn; // function to free buffer
    void *pArg; // param to allocFn
    RecordTbl *pTbl;
    pthread_t thread;
} ABP;

typedef struct abp_st ABP;

// internal funciton declaration

ABP *abp_create(int num, void *(allocFn)(void *), void *pArg) {
    ABP *p = malloc(sizeof(ABP));
    if (p == NULL) {
        goto error0;
    }
    p->num = num;
    p->allocFn = allocFn;
    p->pArg = pArg;
    p->pTbl = rt_create(p->num);
    int ret = pthread_create(&p->thread, NULL, allocRoutineI, p->pArg);
    if (ret != 0) {
        goto error;
    return p;
    void *retptr;
// error4:
    ret = pthread_join(p->thread, &retptr);
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
