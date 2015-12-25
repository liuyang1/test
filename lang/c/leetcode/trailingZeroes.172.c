int trailingZeroes(int n) {
    long long nn;
    int s = 0;
    for (nn = 5; nn <= n; nn *= 5) {
        s += n / nn;
    }
    return s;
}