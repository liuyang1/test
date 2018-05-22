#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>

#include <unistd.h>

int main(int argc, char **argv) {
    FILE *fp;
    if (argc <= 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "rb");
        if (fp == NULL) {
            fprintf(stderr, "fail to open file=%s\n", argv[1]);
            return -1;
        }
    }

#define WORD_LEN_MAX 128
    int wordcount = 0;
    char tmp[WORD_LEN_MAX] = {0};
    int tmpIdx = 0;
    int progress = 0;
    while (true) {
        progress++;
        if (progress % (1000 * 1000) == 0) {
            fprintf(stderr, "process %d bytes data\n", progress);
        }
        char c = fgetc(fp);
        if (!isalpha(c)) {
            if (tmpIdx != 0) {
                // already find a word in tmp
                tmp[tmpIdx] = '\0';
                // printf("find wordcount=%d word=\"%s\"\n", wordcount, tmp);
                printf("%s\n", tmp);
                wordcount++;
                tmpIdx = 0;
            }
            if (c == EOF) {
                break;
            }
            continue;
        }
        c = tolower(c);
        tmp[tmpIdx++] = c;
        if (tmpIdx == WORD_LEN_MAX) {
            fprintf(stderr, "need enlarge tmp word length=%d\n", WORD_LEN_MAX);
            exit(-1);
        }
    }
    fclose(fp);

    return 0;
}
