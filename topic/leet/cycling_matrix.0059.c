/** 输出螺旋矩阵
 *
 * 这里的思路比标准答案还要稍微巧妙一点
 *
 * 1，如何螺旋一圈，这里技巧的是对称性的分割从左到右，从上到下，从右到左，再从上到下的过程
 * 每步填入w-1个元素，四步合计为4*(w-1)方式
 * 这种方式循环的条件非常好写，因为条件是一致的，都是一闭一开区间
 * 2，采用一维数组，而非二维数组的方式来处理。这样可以利用stride，就可以灵活解决向内圈迭代时候的大小减小的问题
 *
 */
#include <stdlib.h>
#include <stdio.h>

void cycling_matrix(int *a, size_t stride, size_t w, int x) {
    if (w == 0) {
        return;
    }
    if (w == 1) {
        a[0] = x;
        return;
    }
    size_t i, j;
    for (i = 0; i != w - 1; i++) {
        a[      0 * stride + i] = x++;
    }
    for (j = 0; j != w - 1; j++) {
        a[      j * stride + w - 1] = x++;
    }
    for (i = w - 1; i != 0; i--) {
        a[(w - 1) * stride + i] = x++;
    }
    for (j = w - 1; j != 0; j--) {
        a[      j * stride + 0] = x++;
    }
    cycling_matrix(a + stride + 1, stride, w - 2, x);
}

void show_2d(int *a, size_t n) {
    size_t i, j;
    for (j = 0; j != n; j++) {
        for (i = 0; i != n; i++) {
            printf("%d, ", a[j * n + i]);
        }
        printf("\n");
    }
    printf("\n");
}

void matrix(int *a, size_t stride) {
    return cycling_matrix(a, stride, stride, 1);
}

int test_4() {
    int a[16] = {0};
    cycling_matrix(a, 4, 4, 1);
    show_2d(a, 4);
    return 0;
}

int main() {
    size_t i;
    for (i = 1; i != 10; i++) {
        int *a = malloc(sizeof(int) * i * i);
        matrix(a, i);
        show_2d(a, i);
        free(a);
    }
    return 0;
}
