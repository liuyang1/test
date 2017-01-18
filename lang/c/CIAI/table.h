#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED

typedef const void* Key_T;
typedef void *Val_T;
typedef int (*CmpFn)(const Key_T, const Key_T);
typedef unsigned int (*HashFn)(const Key_T);

#define T Table_T
typedef struct T *T;

extern T Table_new(int hint,
                   CmpFn cmp,
                   unsigned hash(const void *key));
extern void Table_free(T *table);

extern int Table_length(T table);
// return previsou value if table already holds key
extern Val_T Table_put(T table, Key_T key, Val_T value);
extern Val_T Table_get(T table, Key_T key);
extern Val_T Table_remove(T table, Key_T key);
extern void Table_map(T table,
                      void apply(Key_T key, Val_T *val, void *cl),
                      void *cl);
extern Val_T *Table_toArray(T table, Val_T end);

#undef T

#define T RbTable_T
typedef struct T *T;

extern T RbTable_new(CmpFn cmpFn);
extern void RbTable_free(T *table);

extern int RbTable_length(T table);
extern Val_T RbTable_put(T table, Key_T key, Val_T val);
extern Val_T RbTable_get(T table, Key_T key);
extern Val_T RbTable_remove(T table, Key_T key);

#undef T

#endif
