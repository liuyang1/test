#include <map>
#include <iostream>
#include <string>

using namespace std;

int main() {
    map<string, int> dct;

    dct.insert({"answer of end", 42});
    dct.insert(make_pair("age", 21));

    auto it = dct.find("age");
    cout << "age: " << it->second << endl;

    auto ret = dct.insert(make_pair("age", 18));
    // ret type is
    // pair < iterator< pair<string, int> >, bool >
    if (ret.second == false) {
        cout << "already have AGE as key" << endl;
        cout << "You AGE is " << ret.first->second << endl;
    }

    cout << "loop on MAP" << endl;
    for (auto i = dct.begin(); i != dct.end(); i++) {
        cout << i->first << ": " << i->second << endl;
    }
    return 0;
}
