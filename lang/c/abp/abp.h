#ifndef _ABP_H_
#define _ABP_H_

typedef struct abp_st ABP;

ABP * abp_create(int num, int size);

void abp_labelDestroy(ABP *p);

void abp_waitDestory(ABP *p);

int abp_allocate(ABP *p, void **buf);

int abp_free(ABP *p, void *buf);

unsigned int abp_avaibles(ABP *p);

#endif
