#ifndef _ABP_H_
#define _ABP_H_

typedef struct abp_st *ABP;

// create an ABP, it will NUM buffer, whose size is SIZE.
ABP abp_create(int num, void *(allocFn)(void *), void *pArg);

// must wait every buffer allocate
// all allocated buffer must be free state, then ABP could be destory
void abp_waitDestory(ABP p);

/** allocate one buffer from ABP
 * As ABP is async allocated buffer, so this function will return immdiately if
 * have avaible buffer
 * if not, this function will wait until one free buffer, then return
 *
 * if don't have free buffer, return NULL
 */
int abp_allocate(ABP p, void **buf);

// free one buffer from ABP
int abp_free(ABP p, void *buf);

unsigned int abp_avaibles(ABP p);

#endif
