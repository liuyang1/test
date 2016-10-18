#include "leet.h"
/**
 * Given n non-negative integers a1, a2, ..., an, where each represents a point
 * at coordinate (i, ai). n vertical lines are drawn such that the two
 * endpoints of line i is at (i, ai) and (i, 0).
 * Find two lines, which together with x-axis forms a container, such that the
 * container contains the most water.
 */

#define MIN(a, b) ((a) < (b) ? (a) : (b))
int area(int *h, int a, int b) {
    return MIN(h[a], h[b]) * (b - a);
}

/**
 * Area is calucalute with this formula. so how to find max area?
 * Just asume search from most width [0, n - 1] bar.
 * If move the large bar, area must decrease as height and width decrase same
 * time.
 * If mvoe the small bar, area may increase as width decrease, but height
 * increase.
 * So we just move the small bar to next larger one from endpoint to central.
 */

int maxArea(int *height, int heightSize) {
    int b, e, m, a;
    for (b = 0, e = heightSize - 1, m = 0; b < e;) {
        a = area(height, b, e);
        if (m < a) {
            m = a;
        }
        if (height[b] < height[e]) {
            int old = height[b];
            while (height[++b] < old && b < e) {
            }
        } else {
            int old = height[e];
            while (height[--e] < old && b < e) {
            }
        }
    }
    return m;
}

int test() {
    int hs[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    int e = 49;
    int a = maxArea(hs, COUNT_OF(hs));
    showArr(hs, COUNT_OF(hs));
    printf("maxArea(arr)=%d ?= %d %s\n", a, e, expect(a == e));
    return 0;
}

int main() {
    test();
    return 0;
}
