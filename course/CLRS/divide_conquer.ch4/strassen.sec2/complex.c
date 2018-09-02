#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

complex mul(complex x, complex y) {
    double a = creal(x), b = cimag(x);
    double c = creal(y), d = cimag(y);
    // (a + bi)(c + di) = (ac - bd) + (bc + ad)i
    double ac = a * c;
    double bd = b * d;
    double xx = (a + b) * (c + d);
    return ac - bd + (xx - ac - bd) * I;
}

char *cshow(complex a, char **ps) {
    char *s = malloc(sizeof(char) * 128);
    snprintf(s, 128, "(%f,%f)", creal(a), cimag(a));
    *ps = s;
    return s;
}

int main() {
    complex a = 3 + 4 * I;
    complex b = 5 + 6 * I;
    complex c = mul(a, b);
    complex e = a * b; // C's '*' suport complex number

    char *s[4] = {NULL};
    printf("%s * %s = %s == %s %s\n",
           cshow(a, &s[0]),
           cshow(b, &s[1]),
           cshow(c, &s[2]),
           cshow(e, &s[3]),
           c == e ? "PASS" : "FAIL");
    int i;
    for (i = 0; i != 4; i++) {
        free(s[i]);
    }
    return 0;
}
