#include <stdio.h>

typedef struct {
    int a;
    int b;
} test_st;

#define MemOf(p, prop) (((test_st *)p)->prop)

int main(void) {
    test_st t = {1, 2};
    test_st *p = &t;
    void *vp = p;
    printf("%d \n", MemOf(vp, a));
    MemOf(vp, a) = 3;
    printf("%d \n", MemOf(vp, a));
    return 0;
}
