#include <iostream>
#include <vector>

using namespace std;

class solution {
    public:
        int maxSumArray(vector < int >&nums) {
            auto m = nums[0], mm = m;
            for (auto i = nums.begin() + 1; i != nums.end(); i++) {
                m = max(*i, *i + m);
                mm = max(mm, m);
            }
            return mm;
        }
};

int main() {
    vector < int > nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    auto sln = solution();
    auto m = sln.maxSumArray(nums);
    cout << m << endl;
    return 0;
}
