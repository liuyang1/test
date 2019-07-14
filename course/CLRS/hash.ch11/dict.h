#ifndef __DICT_H__
#define __DICT_H__

typedef const char *Key;
typedef const char *Value;

int dict_create(void **d);
int dict_destroy(void *d);

/** update value, if d[k] exists */
int dict_insert(void *d, Key k, Value v);
int dict_search(void *d, Key k, Value *pv);
int dict_delete(void *d, Key k);

#endif
