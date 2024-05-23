/**
 * @author      : liuyang1 (liuyang1@mail.ustc.edu.cn)
 * @file        : test
 * @created     : Tuesday May 21, 2024 19:41:00 CST
 */

#include "test.h"

// func01(0) 100% statement coverage, 50% branch coverage
void func01(int x) {
    if (x < 3) {
        printf("x is less 3\n");
    }
    printf("exit test 01\n");
}

// func(0, 0), 100% branch coverage, 50% MC/DC coverage
void func02(int x, int y) {
    if (x < 3 && y < 4) {
        printf("x is less than 3 and y is less than 4\n");
    }
    printf("exit func02\n");
}
