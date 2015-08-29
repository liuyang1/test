#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lzw.h"

Entry gTbl[TABLELENGTH];

void lzwCompress(FILE *fs)
{
    unsigned int prev = fgetc(fs);
    unsigned int writeIdx = ENDFLAG;
    unsigned int idx;
    char c;
    initTbl(gTbl);
    while (1) {
        while (1) {
            if (feof(fs)) {
                return;
            }
            c = fgetc(fs);
            if (c == EOF) {
                return;
            }
            idx = searchTbl(gTbl, prev, c);
            if (idx != INVALIDLENGTH) {
                prev = idx;
            } else {
                break;
            }
        }
        outputCom(prev);
        writeTbl(gTbl, writeIdx, prev, c);
        writeIdx++;
        prev = c;
    }
}

void usage(char *name)
{
    printf("usage: %s [filename=stdin]\n", name);
}

int main(int argc, char **argv)
{
    char *fn;
    FILE *fp;
    if (argc > 2) {
        usage(argv[0]);
        return -1;
    } else if (argc == 2) {
        fn = argv[1];
        fp = fopen(fn, "r");
        if (fp == NULL) {
            perror(fn);
            return errno;
        }
    } else {// argc == 1
        fp = stdin;
    }
    lzwCompress(fp);
    fclose(fp);
    return 0;
}
