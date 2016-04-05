#include "leet.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
int iter(int *nums, int i) {
    return i + nums[i];
}

bool canJump(int *nums, int numsSize) {
    int left = 0, right = 0, i, max, v;
    while (1) {
        max = right;
        for (i = left; i <= right; i++) {
            v = iter(nums, i);
            max = MAX(max, v);
        }
        LOG("iter left=%d right=%d max=%d\n", left, right, max);

        // iter to next
        if (max == right || max >= numsSize - 1) {
            break;
        }
        left = right;
        right = max;
    }
    return max >= numsSize - 1;

}

#define CASE(a, e) {bool r = canJump(a, COUNT_OF(a)); \
                    printf("%s ?= %s %s\n",           \
                           SBOOL(r), SBOOL(e), expect(r == e)); }

int main() {
    CASE(((int[]) {0}), true);
    CASE(((int[]) {3, 2, 1, 0, 4}), false);
    CASE(((int[]) {2, 2, 1, 0, 4}), false);
    CASE(((int[]) {3, 0, 0, 0, 4}), false);
    CASE(((int[]) {3, 2, 1, 1, 4}), true);
    CASE(((int[]) {2, 0, 5, 0, 0}), true);
    CASE(((int[]) {1, 3, 2, 1, 1, 4}), true);
    CASE(((int[]) {1, 3, 0, 3, 1, 0, 4}), true);
    return 0;
}
