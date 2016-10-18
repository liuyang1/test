#include "leet.h"

const char digit2lettersTbl[][5] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
const char *digit2letters(char digit) {
    int i = digit - '0';
    if (i < 2 && i > 9) {
        DIE("have no letters for digit=%d\n", i);
        return NULL;
    }
    i -= 2;
    return digit2lettersTbl[i];
}

char **letterCombinations(char *digits, int *returnSize) {
    int n = strlen(digits);
    if (n == 0) {
        *returnSize = 0;
        return NULL;
    }
    char const **arr = malloc(sizeof(*arr) * n);
    int *status = malloc(sizeof(*status) * n);
    int *lens = malloc(sizeof(*lens) * n);
    int i, j, outnum = 1;
    for (i = 0; i != n; i++) {
        arr[i] = digit2letters(digits[i]);
        status[i] = 0;
        lens[i] = strlen(arr[i]);
        outnum *= lens[i];
    }
    char **out = malloc(sizeof(char *) * outnum);

    for (i = 0;; i++) {
        out[i] = malloc(sizeof(char) * (n + 1));
        for (j = 0; j != n; j++) {
            out[i][j] = arr[j][status[j]];
        }
        out[i][j] = '\0';
        if (i == outnum - 1) {
            break;
        }
        status[0] = status[0] + 1;
        for (j = 0; j != n; j++) {
            if (status[j] != lens[j]) {
                break;
            }
            status[j] = 0;
            status[j + 1]++;
        }
    }
    free(status);
    free(lens);
    free(arr);
    *returnSize = outnum;
    return out;
}

int main() {
    char digits[] = "239";
    int len;
    char **r = letterCombinations(digits, &len);
    showStr2(r, len);
    return 0;
}
