void squeeze_c(char s[], char c) {
    int i, j;
    for (i = j = 0; s[i] != '\0'; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
}
void squeeze(char s1[], char s2[]) {
    for (; *s2 != '\0'; s2++) {
        squeeze_c(s1, *s);
    }
}
