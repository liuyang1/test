int lengthOfLastWord(char *s) {
    char *p;
    do {
        p = strrchr(s, ' ');
        if (p == NULL) {
            return strlen(s);
        }
        if (p[1] == '\0') {
            p[0] = '\0';
            continue;
        } else {
            return strlen(p + 1);
        }
    } while (1);
}
