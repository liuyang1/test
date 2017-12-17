/** predicate function group */
int islower(int c) {
    return c >= 'a' && c <= 'z';
}
int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}
int isdigit(int c) {
    return c >= '0' && c <= '9';
}
int isxdigit(int c) {
    c = toupper(c);
    return isdigit(c) || (c >= 'A' && c <= 'F');
}
int isalpha(int c) {
    return islower(c) || isupper(c);
}
int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}
int isascii(int c) {
    return c >= 0 && c <= 255;
}
/** convert function */
int toupper(int c) {
    if (islower(c)) {
        c += 'A' - 'a';
    }
    return c;
}

int tolower(int c) {
    if (isupper(c)) {
        c += 'a' - 'A';
    }
    return c;
}
