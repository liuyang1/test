int lengthOfLongestSubstring(char *s) {
    int i, len = strlen(s);
    if (len <= 1) {
        return len;
    }
    bool *aIsRepeat = (bool *)malloc(sizeof(bool) * len);
    for (i = 0; i != len; i++) {
        aIsRepeat[i] = false;
    }

    int thresh = 1;
    bool find = true;
    // Only IF find a[i, i+t] diff, and t != len, THEN continue find
    while (find && thresh != len) {
        for (i = 0, find = false; i != len - thresh; i++) {
            if (aIsRepeat[i] == true) {
                continue;
            }
            // IF a[i, i+t] diff AND a[i+1, i+t+1] diff AND a[i], a[i+t] diff,
            //      THEN, a[i, i+t+1] diff.
            if (aIsRepeat[i + 1] == true || s[i] == s[i + thresh]) {
                aIsRepeat[i] = true;
            } else if (find == false) {
                find = true;
            }
        }
        if (find) {
            thresh++;
        }
    }
    free(aIsRepeat);
    return thresh;
}
