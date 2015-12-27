bool isIsomorphic(char* s, char* t) {
    if (s == NULL || t == NULL) {
        if (s == NULL && t == NULL) {
            return true;
        }
        return false;
    }
    #define CHARSETSZ 256
    char map[CHARSETSZ] = {0};
    int i;
    for (; *s != '\0' && *t != '\0'; s++, t++) {
        if (map[*s] == 0) {
            // checking only one char map to *T
            for (i = 0; i != CHARSETSZ; i++) {
                if (map[i] == *t) {
                    return false;
                }
            }
            map[*s] = *t;
        } else if (map[*s] != *t) {
            // checking all char *S map to *T
            return false;
        }
    }
    return *s == '\0' && *t == '\0';
}
