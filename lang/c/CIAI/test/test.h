#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define EXPECT_PTR_EQ(a, b) {if (a != b) { \
    printf("%s():%d %p != %p %s\n", __func__, __LINE__, a, b, "FAIL"); \
    return false; \
}}
#define ASSERT(b) {if(!b) {printf("%s FAIL\nassert!\n", #b); assert(0);}}

#endif
