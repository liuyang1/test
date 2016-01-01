int hammingWeight(uint32_t n) {
    int cnt;
    for (cnt = 0; n != 0; n &= n - 1, cnt++) {
    }
    return cnt;
}
