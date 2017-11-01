#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

static long findMissing(vector < long > list) {
    if (list.size() < 3) {
        throw "invalid list, must longer than 3";
    }
    for (size_t i = 0; i != list.size() - 2; i++) {
        auto a0 = list[i], a1 = list[i + 1], a2 = list[i + 2];
        if (a2 - a1 != a1 - a0) {
            return a2 + a0 - a1;
        }
    }
    throw "invalid list, cannot find missing element";
}

// below test code
string toString(vector < long > list) {
    ostringstream oss;
    oss << "[";
    bool first = true;
    for (auto i: list) {
        if (!first) {
            oss << ", ";
        }
        first = false;
        oss << i;
    }
    oss << "]";

    return oss.str();
}

bool test_findMissing(vector < long > list, long expect) {
    auto r = expect == findMissing(list);
    string rs = r ? "==" : "!=";
    cout << "findMissing(" << toString(list) << ") " << rs << " " << expect << endl;
    return r;
}

bool test_reversed(vector < long > list, long expect) {
    auto rev = list;
    reverse(rev.begin(), rev.end());
    return test_findMissing(list, expect) && test_findMissing(rev, expect);
}

bool test_execpt() {
    try {
        vector < long > list0 {0, 1};
        findMissing(list0);
    } catch(char const *s) {
        cerr << "Except: " << s << endl;
    }

    try {
        vector < long > list0 {0, 0, 0};
        findMissing(list0);
    } catch(char const *s) {
        cerr << "Except: " << s << endl;
    }

    try {
        vector < long > list0 {1, 2, 3};
        findMissing(list0);
    } catch(char const *s) {
        cerr << "Except: " << s << endl;
    }
    return true;
}

int main() {
    vector < long > list0 {1, 2, 4};
    test_reversed(list0, 3);
    vector < long > list1 {1, 3, 4};
    test_reversed(list1, 2);

    test_execpt();
    return 0;
}
