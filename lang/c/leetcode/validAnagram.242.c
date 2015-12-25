#define CHARSETSIZE 26
void statStr(char *s, int *a)
{
    int i;
    for (i = 0; i < CHARSETSIZE; i++) {
        a[i] = 0;
    }
    for (; *s != '\0'; s++) {
        a[*s - 'a']++;
    }
}

bool isAnagram(char* s, char* t) {
    int a0[CHARSETSIZE];
    int a1[CHARSETSIZE];
    statStr(s, a0);
    statStr(t, a1);
    int i;
    for (i = 0; i < CHARSETSIZE; i++) {
        if (a0[i] != a1[i]) {
            return false;
        }
    }
    return true;
}
