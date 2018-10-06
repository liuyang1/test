#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main() {
    char *s = malloc(sizeof(char) * 1024);
    while (true) {
        /** I'm sorry, this out-dated ZJU's OJ only support C 4.7.2
            It doesn't support modern feature listed below:
            * ssize_t type
            * getline (POSIX 2008)
            * variable declaration at anywehre
            * It even cannot support comment for line
        */
        if (scanf("%s", s) == EOF) {
            break;
        }
        {
            int sz = strlen(s);
            int len = 3 * (sz - 2) + 1;
            char *dst = malloc(sizeof(char) * len);
            int i, j;
            int validDigits = 0;
            for (i = sz - 1; i != 1; i--) {
                int num = s[i] - '0';
                for (j = 0; j < validDigits || num != 0; j++) {
                    int t = num * 10;
                    if (j < validDigits) {
                        t += dst[j] - '0';
                    }
                    dst[j] = t / 8 + '0';
                    num = t % 8;
                }
                validDigits = j;
            }
            dst[len - 1] = '\0';
            printf("%s [8] = 0.%s [10]\n", s, dst);
            free(dst);
        }
    }
    free(s);
    return 0;
}
