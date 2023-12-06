#include <iostream>
#include <vector>
#include <algorithm>


std::vector<size_t> build(const std::string& s) {
    std::vector<std::pair<std::string, size_t> > suffixes;
    for (size_t i = 0; i < s.length(); ++i) {
        suffixes.emplace_back(s.substr(i), i);
        std::cout << s.substr(i) << i << std::endl;
    }

    std::sort(suffixes.begin(), suffixes.end());

    std::vector<size_t> sa(s.length());
    for (size_t i = 0; i < s.length(); ++i) {
        sa[i] = suffixes[i].second;
        std::cout << suffixes[i].first << suffixes[i].second << std::endl;
    }
    return sa;
}

int main() {
    std::string s = "ecnu";
    build(s);
    s = "cubercsl";
    build(s);
    return 0;
}
