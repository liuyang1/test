#include <iostream>
#include <string>

using namespace std;
/** without duplicated char */
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        size_t m = 0;
        ssize_t pos[256] = {-1}, i;
        for (i = 0; s[i] != '\0'; i++) {
            int idx = s[i];
            size_t d = i - pos[idx];
            if (d > m) {
                m = d;
                printf("m=%d c=%c\n", m, s[i]);
            }
            pos[idx] = i;
        }
        return m;
    }
};

void test(string s) {
    Solution sln = Solution();
    int len = sln.lengthOfLongestSubstring(s);
    cout << s << ": " << len << endl;
}

int main() {
    test("abcabcabc");
    test("helloworld");
    return 0;
}
