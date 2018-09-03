#include <vector>
#include <iostream>
#include <climits>

using namespace std;
class Solution {
    public:
        class Item {
            public:
                long int freq;
                vector < int > ::iterator begin, end;
                Item() {
                    freq = 0;
                }
        };
        /**
         * nums.length will be between 1 and 50,000
         * nums[i] will be an integer between 0 and 49,999.
         */
        static int findShortestSubArray(vector < int >&nums) {
            static const int MAXVALUE = 50000;
            vector < Item > t(MAXVALUE);
            for (auto i = nums.begin(); i != nums.end(); i++) {
                auto p = &t[*i];
                if (p->freq == 0) {
                    p->begin = i;
                }
                p->freq++;
                p->end = i;
            }
            auto mfreq = 0l, mindist = LONG_MAX;
            for (auto i: t) {
                mfreq = max(i.freq, mfreq);
            }
            for (auto i: t) {
                if (i.freq == mfreq) {
                    auto dist = i.end - i.begin + 1;
                    mindist = min(dist, mindist);
                }
            }
            return mindist;
        }

};

int main() {
    vector < int > a = {1, 2, 2, 3, 1, 4, 2};
    int v = Solution::findShortestSubArray(a);
    cout << v << endl;
    return 0;
}
