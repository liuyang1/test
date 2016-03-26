#include "leet.h"
int findDuplicate(int *nums, int numsSize) {
    int slow, fast;
    slow = fast = 0;
    while (true) {
        slow = nums[slow];
        fast = nums[nums[fast]];
        if (slow == fast) {
            break;
        }
    }
    int finder = 0;
    while (true) {
        slow = nums[slow];
        finder = nums[finder];
        if (slow == finder) {
            return finder;
        }
    }
}

#define CASE(a, e) {int r = findDuplicate(a, sizeof(a) / sizeof(a[0])); \
                    printf("%d ?= %d %s\n", r, e, expect(r == e)); }
int main() {
    CASE(((int[]) {1, 1}), 1);
    CASE(((int[]) {1, 2, 2}), 2);
    CASE(((int[]) {1, 2, 2, 4, 5}), 2);
    CASE(((int[]) {1, 2, 2, 2, 5}), 2);
    CASE(((int[]) {1, 2, 2, 2, 2}), 2);
    CASE(((int[]) {2, 2, 2, 2, 2}), 2);
    CASE(((int[]) {1, 2, 3, 4, 3}), 3);
    return 0;
}
