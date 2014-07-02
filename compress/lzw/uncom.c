#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "lzw.h"

Entry gTbl[TABLELENGTH];

void lzwUncompress(FILE* fs)
{
    unsigned int prev = INVALIDLENGTH;
    unsigned int writeIdx = ENDFLAG;
    unsigned int idx;
    unsigned int code, code0, code1;
    initTbl(gTbl);
    while(1)
    {
        while(1){
            code0 = fgetc(fs);
            code1 = fgetc(fs);
            if(code0 == EOF)
                return;
            if(code1 == EOF){
                perror("file was corrupted, size is not even\n");
                return;
            }
            code = code0<<8 | code1;
            printf("0x%04x\n", code);
        }
    }
}

void usage(char *name)
{
    printf("usage: %s [filename=stdin]\n", name);
}

int main(int argc, char** argv)
{
    char* fn;
    FILE* fp;
    if(argc > 2){
        usage(argv[0]);
        return -1;
    }else if(argc == 2){
        fn = argv[1];
        fp = fopen(fn, "rb");
        if(fp == NULL){
            perror(fn);
            return errno;
        }
    }else{
        fp = stdin;
    }
    lzwUncompress(fp);
    fclose(fp);
    return 0;
}
