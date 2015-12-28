char* convert(char* s, int numRows) {
    if (numRows <= 1) {
        return s;
    }
    int len = strlen(s);
    char *r = malloc(sizeof(char) * (len + 1));
    int i, j, k, idx = 0;
    /*
     * 0     6
     * 1   5 7
     * 2 4   8 10
     * 3     9
     */
    for (i = 0, k = 0; i != numRows; i++) {
        for (j = i; j < len; j += 2 * numRows - 2) {
            r[k] = s[j];
            k++;
            if (i != 0 && i != numRows - 1 && j + 2 * (numRows - i - 1) < len) {
                r[k] = s[j + 2 * (numRows - i - 1)];
                k++;
            }
        }
    }
    r[k] = '\0';
    return r;
}
