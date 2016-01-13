double myPow(double x, int n) {
    double r;
    if (x == 0) {
        return 0;
    }
    if (n == 0) {
        return 1;
    } else if (n > 0) {
        r = myPow(x, n / 2);
        r *= r;
        if (n % 2 == 0) {
            return r;
        } else {
            return r * x;
        }
    } else {
        r = myPow(x, n / 2);
        r *= r;
        if (n % 2 == 0) {
            return r;
        } else {
            return r / x;
        }
    }
}
