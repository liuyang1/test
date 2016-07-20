#include "../kr.h"

// signed integer is two-complement representation.
// so INT_MIN will overflow.

void reverse_r_i(char *s, int len, int idx) {
    if (idx < 0) {
        return;
    }
    char t;
    t = s[idx];
    s[idx] = s[len - 1 - idx];
    s[len - 1 - idx] = t;
    reverse_r_i(s, len, idx - 1);
}

void reverse_r(char *s) {
    int len = strlen(s);
    int half = len / 2 - 1;
    reverse_r_i(s, len, half);
}

#define CASE(s) {char *r = strdup(s); reverse_r(r);          \
                 char *e = strdup(s); reverse(e);            \
                 printf("%s -> %s ?= %s %s\n",               \
                        s, r, e, expect(strcmp(r, e) == 0)); \
                 free(e); free(r);                           \
}

int main() {
    CASE("hello");
    CASE("yeah");
    CASE("complex one case");
    return 0;
}
