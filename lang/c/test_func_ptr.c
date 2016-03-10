#include <stdio.h>

typedef int (*int_func_type)(int);

int func(int a) {
    return a * 2;
}

int main() {
    int_func_type fn = func;
    printf("int_func_type fn=%p\n", fn);
    int a = 21;
    printf("fn(%d) = %d\n", a, fn(a));

    fn = NULL;
    // pFn is pointer to function pointer
    int_func_type *pFn = &fn;;
    printf("int_func_type *pFn=%p *pFn=%p\n", pFn, *pFn);

    *pFn = &func;
    printf("int_func_type *pFn=%p *pFn=%p\n", pFn, *pFn);
    printf("*pFn(%d) = %d\n", a, (*pFn)(a));

    return 0;
}
