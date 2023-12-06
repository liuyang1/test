#include <stdio.h>

#define N       101

int ybb_count(char *s) {
    int cnt = 0;
    int st = -1;
    for (; *s != '\0'; s++) {
        if (*s == 'y') {
            st = 0;
        } else if (*s == 'b' && st == 0) {
            st = 1;
        } else if (*s == 'b' && st == 1) {
            cnt++;
        } else {
            st = -1;
        }
    }
    return cnt;
}

int main() {
    int t;
    scanf("%d" ,&t);
    for (; t != 0; t--) {
        int n;
        char s[N];
        scanf("%d", &n);
        scanf("%s", s);
        printf("%d\n", ybb_count(s));
    }
    return 0;
}
