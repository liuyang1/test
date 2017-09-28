int climbStairs(int n) {
    if (n <= 1) {
        return 1;
    }
    int a0 = 1, a1 = 1, t;
    for (; n != 1; n--) {
        t = a1;
        a1 = a1 + a0;
        a0 = t;
    }
    return a1;
}
