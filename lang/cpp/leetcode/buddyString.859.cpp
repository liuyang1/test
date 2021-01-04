#include <string>
#include <iostream>
using namespace std;
class Solution {
    public:
        /**
         * same length
         * only two letter different (with index), swap, same
         * all same, and have two sam letter
         * !!! consider '\0' as ending of string, instead of cend() '\0'
         */
        bool buddyStrings(string A, string B) {
            char x = '\0', y = '\0';
            size_t cnt = 0;
            auto i = A.cbegin(), j = B.cbegin();
            for (;
                 *i != '\0' && *j != '\0';
                 i++, j++) {
                if (*i == *j) {
                    continue;
                }
                cnt++;
                if (cnt == 1) {
                    x = *i, y = *j;
                } else if (cnt == 2){
                    if (*i != y || *j != x) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
            if (i != A.cend() || j != B.cend()) {
                return false;
            }
            if (cnt == 2) {
                return true;
            }
            if (cnt == 0) {
                for (auto i = A.cbegin(); *(i + 1) != '\0'; i++) {
                    for (auto j = i + 1; *j != '\0'; j++) {
                        if (*i == *j) {
                            return true;
                        }
                    }
                }
                return false;
            }
            return false;
        }
};

void test(string A, string B, bool expect) {
    Solution sln = Solution();
    bool r = sln.buddyStrings(A, B);
    cout << A << ", " << B
        << ": " << r
        << ": " << (expect == r ? "pass" : "fail") << endl;
}
int main() {
    test("ab", "ba", true);
    test("ab", "ab", false);
    test("aa", "aa", true);
    test("aaaabc", "aaaacb", true);
    test("", "aa", false);
    test("", "", false);
    return 0;
}
