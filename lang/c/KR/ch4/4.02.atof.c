#include "../kr.h"

double myAtof(char s[]) {
    double val, power;
    int i, sign;

    for (i = 0; isspace(s[i]); i++) {
        ;
    }
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-') {
        i++;
    }
    for (val = 0.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
    }
    if (s[i] == '.') {
        s++;
    }
    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        int expSign = (s[i] == '-') ? -1 : 1;
        if (s[i] == '+' || s[i] == '-') {
            i++;
        }
        double expPower;
        for (expPower = 0.0; isdigit(s[i]); i++) {
            expPower = 10.0 * expPower + (s[i] - '0');
        }
        power /= pow(10.0, expSign * expPower);
    }
    return sign * val / power;
}

#define CASE(s) {double r = myAtof(s), e = strtod(s, NULL); \
                 printf("%f %f %s\n", r, e, expect(eqDouble(r, e))); }

int main() {
    CASE("12");
    CASE("1.2");
    CASE("1.2e3");
    CASE("-1.2e-3");
    CASE("1.e2");
    CASE("1e-3");
    return 0;
}
