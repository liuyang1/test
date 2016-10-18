#include "test.h"
#include "list.h"

bool checkArrayFromOne(void **p, long int n, void *end) {
    long i;
    for (i = 1;; i++) {
        if (p[i - 1] == end) {
            break;
        }
        EXPECT_PTR_EQ(p[i - 1], (void *)i);
    }
    return i - 1 == n;
}

bool testListCons() {
    List_T lst = List_list((void *)1, 2, 3, NULL);
    void *end = (void *)0xff;
    void **p = List_toArray(lst, end);
    bool r = checkArrayFromOne(p, 3, end);
    List_free(&lst);
    return r;
}

bool testListAppend() {
    List_T lst0 = List_list((void *)1, 2, 3, NULL);
    List_T lst1 = List_list((void *)4, 5, 6, 7, NULL);
    List_T lst = List_append(lst0, lst1);
    void *end = (void *)0xff;
    void **p = List_toArray(lst, end);
    bool r = checkArrayFromOne(p, 7, end);
    List_free(&lst);
    return r;
}

bool testListCopy() {
    List_T lst0 = List_list((void *)1, 2, 3, NULL);
    List_T lst1 = List_copy(lst0);
    List_free(&lst0);
    List_free(&lst1);
    return true;
}

#define NUM         10
bool testListPush() {
    List_T lst = List_list(NULL);
    long i;
    for (i = 1; i != NUM + 1; i++) {
        lst = List_push(lst, (void *)i);
    }
    for (i = NUM; i != 0; i--) {
        void *x;
        lst = List_pop(lst, &x);
        EXPECT_PTR_EQ(x, (void *)i);
    }
    List_free(&lst);
    return true;
}

bool testListLisp() {
    List_T nil = List_list(NULL);
    List_T lst = nil, origin;
    long i;
    for (i = 1; i != NUM + 1; i++) {
        lst = List_cons((void *)i, lst);
    }
    origin = lst;
    for (i = NUM; i != 0; i--, lst = List_cdr(lst)) {
        void *x = List_car(lst);
        EXPECT_PTR_EQ(x, (void *)i);
    }
    // Lisp's style in C memory management is tricky.
    List_free(&origin);
    return true;
}

bool testList() {
    ASSERT(testListCons());
    ASSERT(testListAppend());
    ASSERT(testListCopy());
    ASSERT(testListPush());
    ASSERT(testListLisp());
    return true;
}
