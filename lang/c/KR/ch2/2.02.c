// read one line from STDIN, to buffer which max length is LIM.
// return line length
int func(char *s, int lim) {
    int i, c;
    for (i = 0; i < lim - 1 && (c = getchar()) != '\n' && c != EOF; ++i) {
        s[i] = c;
    }
    s[i] = '\0';
    return i;
}

// 2-02 rewrite this function without &&
int func1(char *s, int lim) {
    int i, c;
    for (i = 0; i < lim - 1; ++i) {
        // split two more IF-statement, and have to use NEGATIVE-logic
        if ((c = getchar()) == '\n') {
            break;
        }
        if (c == EOF) {
            break;
        }
        s[i] = c;
    }
    s[i] = '\0';
    return i;
}
