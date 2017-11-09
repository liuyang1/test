#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// base function
double fabs(double x) {
    return x > 0 ? x : -x;
}

int sign(double x) {
    return (x > 0) - (x < 0);
}

double square(double x) {
    return x * x;
}

double cubic(double x) {
    return x * x * x;
}

// function in math.h

const double pi = 3.1415926;
double sin_p(double x) {
    if (fabs(x) < 0.0001) {
        return x;
    }
    double sx = sin_p(x / 3);
    return 3 * sx - 4 * cubic(sx);
}

double cos_p(double x) {
    return sin_p(pi / 2 - x);
}

void sincos_p(double x, double *psin, double *pcos) {
    *psin = sin_p(x);
    *pcos = cos_p(x);
}

double tan_p(double x) {
    return sin_p(x) / cos_p(x);
}

// TODO:
// acos
// asin
// atan

double ceil_p(double x) {
    return 0;
}

double floor_p(double x) {

    return 0;
}

double trunc(double x) {

    return 0;
}

double fmod_p(double x, double y) {
    return 0;

}

/** extract signed integeral and factional values from floating-point number */
double modf_p(double x, double *iptr) {
    return 0;
}

double copysign_p(double x, double y) {
    return fabs(x) * sign(y);
}

/** convert floating-number to fractional and integral components */
double frexp_p(double x, int *exp) {
    return 0;

}

double sqrt_p(double x) {
    return 0;

}
/** Euclidean distance function */
double hypot(double x, double y) {
    return sqrt_p(square(x) + square(y));
}

const double e = 2.7182818;
double exp_p(double x) {
    return 0;

}

double log_p(double x) {
    return 0;

}

double pow_p(double x) {
    return 0;

}
// test code

bool unit_func(double (*f)(double), double (*std)(double),
               const char *funcname, double x) {
    double r = f(x);
    double e = std(x);
    double d = r - e;
    bool ret = fabs(d) < 0.0001;
    if (!ret) {
        printf("%s_p(%f) = %f %s(%f) = %f, diff=%f\n", funcname, x, r, funcname, x, e, d);
    }
    return ret;
}

bool unit_func_group(double x) {
    return unit_func(sin_p, sin, "sin", x) &&
           unit_func(cos_p, cos, "cos", x) &&
           unit_func(tan_p, tan, "tan", x);

}

int test_func() {
    unit_func_group(0.0001);
    unit_func_group(pi + 1);
    unit_func_group(2 * pi + 1);
    unit_func_group(-pi + 1);
    unit_func_group(-2 * pi + 1);
    unit_func_group(12.15);
    return 0;
}

int main() {
    test_func();
    return 0;
}
