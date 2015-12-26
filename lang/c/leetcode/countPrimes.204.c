int countPrimes(int n) {
    bool *f = malloc(sizeof(bool) * n);
    f[0] = false;
    f[1] = false;
    int i, j, cnt = 0;
    for (i = 2; i < n; i++) {
        f[i] = true;
    }
    for (i = 2; i < n; i++) {
        if (f[2] == false) {
            continue;
        }
        // f[i] = true;
        if (i * i > n) {
            break;
        }
        for (j = i * i; j < n; j += i) {
            f[j] = false;
        }
    }
    cnt = 0;
    for (i = 2; i < n; i++) {
        if (f[i] == true) {
            cnt++;
        }
    }
    return cnt;
}
