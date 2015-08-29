#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lzw.h"

Entry gTbl[TABLELENGTH];
unsigned int writeIdx = ENDFLAG;

unsigned int unpack(Entry *pTbl, unsigned int prev, unsigned int i)
{
    if (i < 256) {
        writeTbl(pTbl, writeIdx, prev, i);
        writeIdx++;
        return i;
    }
    unpack(pTbl, prev, gTbl[i].prev);
    return i;
}

void lzwUncompress(FILE *fs)
{
    unsigned int prev = INVALIDLENGTH;
    unsigned int code, code0, code1;
    initTbl(gTbl);
    code0 = fgetc(fs);
    code1 = fgetc(fs);
    code = code0 << 8 | code1;
    outputUncom(gTbl, code);
    prev = code;
    while (1) {
        code0 = fgetc(fs);
        code1 = fgetc(fs);
        if (feof(fs)) {
            return;
        }
        code = code0 << 8 | code1;
        outputUncom(gTbl, code);
        prev = unpack(gTbl, prev, code);
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
    } else if (argc == 2)   {
        fn = argv[1];
        fp = fopen(fn, "rb");
        if (fp == NULL) {
            perror(fn);
            return errno;
        }
    } else {
        fp = stdin;
    }
    lzwUncompress(fp);
    fclose(fp);
    return 0;
}
