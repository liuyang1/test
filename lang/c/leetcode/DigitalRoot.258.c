int addDigits(int num) {
    // - all number roundly map to [1, 9] , except zero map to zero.
    return 1 + ((num - 1) / 9);
}
