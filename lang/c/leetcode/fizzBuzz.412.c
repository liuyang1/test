#include "leet.h"

char **fizzBuzz(int n, int *returnSize) {
    const int intlen = 11;
    *returnSize = n;
    char **ret = malloc(sizeof(char *) * n);
    int i;
    for (i = 0; i != n; i++) {
        ret[i] = malloc(sizeof(char) * intlen);
        int num = i + 1;
        if (num % 3 == 0 && num % 5 == 0) {
            strcpy(ret[i], "FizzBuzz");
        } else if (num % 3 == 0) {
            strcpy(ret[i], "Fizz");
        } else if (num %  5 == 0) {
            strcpy(ret[i], "Buzz");
        } else {
            sprintf(ret[i], "%d", num);
        }
    }
    return ret;
}

int main() {
    int len;
    char **r = fizzBuzz(20, &len);
    showStr2(r, len);
    int i;
    for (i = 0; i != len; i++) {
        free(r[i]);
    }
    free(r);
    return 0;
}
