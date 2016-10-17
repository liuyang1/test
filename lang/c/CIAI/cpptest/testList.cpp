#include "gtest/gtest.h"
#include "list.h"

TEST(List, cons) {
    List lst = List_list(0, 1, 2, 3);
    void *end = 0xff;
    void **p = List_toArray(lst, end);
    int i;
    for (i = 0; ;i++) {
        if (p[i] == end) {
            break;
        }
        EXPECT_EQ(p[i], i);
    }
}

