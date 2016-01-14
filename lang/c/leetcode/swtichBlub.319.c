int bulbSwitch(int n) {
    if (n <= 0) {
        return 0;
    }
    unsigned long long b, e, m, m2, m12;
    for (b = 1, e = 65535; b < e; ) {
        m = (b + e) / 2;
        m2 = m * m;
        m12 = m2 + 2 * m + 1;
        if (m2 <= n && n < m12) {
            return m;
        } else if (n < m2) {
            e = m - 1;
        } else {
            b = m + 1;
        }
    }
}
