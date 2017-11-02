#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define check_ret(r) {                                   \
        bool _v = r;                                     \
        if (!_v) {                                       \
            /* printf("fail at line:%d\n", __LINE__); */ \
            return _v;                                   \
        }                                                \
}

bool is_valid_zeros(const char *s) {
    return !(s[0] == '0' && isdigit(s[1]));
}

bool eat_valid_num(const char *s, const char **pp) {
    check_ret(is_valid_zeros(s));
    const char *p;
    int v;
    for (v = 0, p = s; isdigit(*p); p++) {
        v = 10 * v + (*p - '0');
    }
    *pp = p;
    return v <= 255 && p != s;
}

bool eat_char(const char *s, char c, const char **pp) {
    *pp = s + 1;
    return *s == c;
}

/* Return 1 is s is a valid IP sess, return 0 otherwise */
// combine paser with monad style
int is_valid_ip(const char *s) {
    int i;
    for (i = 0;; i++) {
        check_ret(eat_valid_num(s, &s));
        if (i == 3) {
            break;
        }
        check_ret(eat_char(s, '.', &s));
    }
    check_ret(eat_char(s, '\0', &s));
    return true;
}

// test code
int unit(const char *s, int expect) {
    int r = is_valid_ip(s);
    bool ret = r == expect;
    const char *sym = ret ? "==" : "/=";
    printf("is_valid_ip(\"%s\") = %d %s %d\n", s, r, sym, expect);
    return ret;
}

int test() {
    unit("12.255.56.1", 1);
    unit("12.255.0.1", 1);
    unit("12.256.0.1", 0);
    unit("", 0);
    unit("abc.def.ghi.jkl", 0);
    unit("123.456.789.0", 0);
    unit("12.34.56", 0);
    unit("12.34.56 .1", 0);
    unit("12.34.56.-1", 0);
    unit("123.045.067.089", 0); // invalid zero case
    unit("123", 0);
    unit("12.34.56.1.1", 0); // more char
    unit("12.34.56.1.", 0); // more char
    unit("12.34..1", 0); // no number
    return 0;
}

int main() {
    test();
    return 0;
}
