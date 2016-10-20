// Forward declaration of guess API.
// @param num, your guess
// @return -1 if my number is lower, 1 if my number is higher, otherwise return 0
int guess(int num) {
    int target = 1702766719;
    if (target < num) {
        return -1;
    } else if (num == target) {
        return 0;
    } else {
        return 1;
    }
}

class Solution {
    public:
        int guessNumber(int n) {
            int b, e, m;
            for (b = 1, e = n; b <= e;) {
                // why need this format?
                // if may overflow when b + e
                m = b + (e - b) / 2;
                int r = guess(m);
                if (r == 1) {
                    b = m + 1;
                } else if (r == 0) {
                    return m;
                } else {
                    e = m - 1;
                }
            }
            return -1;
        }

};

#include <stdio.h>

int testOverflow() {
    Solution sln;
    int r = sln.guessNumber(2126753390);
    printf("%d\n", r);
    return 0;
}

int main() {
    testOverflow();
    return 0;
}
