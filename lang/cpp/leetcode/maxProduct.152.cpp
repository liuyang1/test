#include <iostream>
#include <vector>
using namespace std;

/** This is dervied from max subarray sum problem.
 */

class Solution {
    public:
        int maxProduct(vector < int >&nums) {
            int m0, m1, mm;
            m0 = m1 = mm = nums[0];
            for (auto i = nums.begin() + 1; i != nums.end(); i++) {
                m0 = max(*i, *i * m0);
                m1 = min(*i, *i * m1);
                mm = max(mm, m1);
                mm = max(mm, m0);
                cout << m0 << " " << m1 << " " << mm << endl;
            }
            return mm;
        }
};

int main() {
    Solution sln;
    vector <int> a0 = {2, 3, -2, 4};
    cout << sln.maxProduct(a0) << endl;
    vector <int> a1 = {-2, 0, -1};
    cout << sln.maxProduct(a1) << endl;
    vector <int> a2 = {-2, 3, -4};
    cout << sln.maxProduct(a2) << endl;
    return 0;
}
