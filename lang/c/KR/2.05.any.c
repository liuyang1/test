int any_c(char c, const char s2[]) {
    int i;
    for (i = 0; s2[i] != '\0'; i++) {
        if (s2[i] == c) {
            return i;
        }
    }
    return -1;
}
int any(const char s1[], const char s2[]) {
    int i;
    for (i = 0; s1[i] != '\0'; i++) {
        if (-1 != any_c(s1[i], s2)) {
            return i;
        }
    }
    return -1;
}
