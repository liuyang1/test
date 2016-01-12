#include "leet.h"
bool isPalindrome(char* s) {
    int len = strlen(s);
    char *h = s, *t = s + len - 1;
    while (h < t) {
        if (!isalnum(*h)) {
            h++;
            continue;
        }
        if (!isalnum(*t)) {
            t--;
            continue;
        }
        if (tolower(*h) != tolower(*t)) {
            return false;
        }
        h++;
        t--;
    }
    return true;
}

#define CASE(s, e) {bool r = isPalindrome(s); \
    printf("[%s] %s ?= %s %s\n", s, SBOOL(r), SBOOL(e), expect(r == e)); }
int main() {
    CASE("", true);
    CASE("a", true);
    CASE("aa", true);
    CASE("aba", true);
    CASE("abba", true);
    CASE("abaa", false);
    CASE("A man, a plan, a canal: Panama", true);
    CASE("race a car", false);
    return 0;
}
