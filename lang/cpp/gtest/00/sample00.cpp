/**
 * @author      : lyz (lyz@$HOSTNAME)
 * @file        : sample00
 * @created     : Saturday Oct 12, 2024 13:51:53 CST
 */

#include <stdio.h>

#include "gtest/gtest.h"

TEST(all, hello) {
    printf("method=%s\n",
           ::testing::UnitTest::GetInstance()->current_test_info()->name());
    int x = 42;
    EXPECT_EQ(x, 42);
}
