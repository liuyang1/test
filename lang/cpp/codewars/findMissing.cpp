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
string show(vector < long > list) {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i != list.size(); i++) {
        if (i != 0) {
            oss << ", ";
        }
        oss << list[i];
    }
    oss << "]";

    return oss.str();
}

bool test_findMissing(vector < long > list, long expect) {
    auto r = expect == findMissing(list);
    string rs = r ? "==" : "!=";
    cout << "findMissing(" << show(list) << ") " << rs << " " << expect << endl;
    return r;
}

bool test_reversed(vector < long > list, long expect) {
    auto rev = list;
    reverse(rev.begin(), rev.end());
    return test_findMissing(list, expect) && test_findMissing(rev, expect);
}

bool test_execpt(vector < long > list) {
    try {
        findMissing(list);
    } catch(char const *s) {
        cerr << "Except: " << s << ", when handle " << show(list) << endl;
        return true;
    }
    return false;
}

int main() {
    vector < long > list0 {1, 2, 4};
    test_reversed(list0, 3);
    vector < long > list1 {1, 3, 4};
    test_reversed(list1, 2);

    vector < long > list2 {0, 1}, list3 {1, 2, 3}, list4 {0, 0, 0};
    test_execpt(list2);
    test_execpt(list3);
    test_execpt(list4);
    return 0;
}
