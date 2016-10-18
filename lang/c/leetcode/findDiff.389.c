#include "leet.h"

void statString(char *s, int *cnt) {
    for (; *s; s++) {
        cnt[*s - 'a']++;
    }
}

#define NUM 26
char findTheDifference(char *s, char *t) {
    int cnt0[NUM] = {0}, cnt1[NUM] = {0};
    statString(s, cnt0);
    statString(t, cnt1);
    int i;
    for (i = 0; i != NUM; i++) {
        if (cnt0[i] != cnt1[i]) {
            return i + 'a';
        }
    }
    return '0';
}

int main() {
    char c = findTheDifference("abcd", "abcde");
    printf("diff: %c\n", c);
    return 0;
}
