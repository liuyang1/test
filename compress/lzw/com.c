#include <stdio.h>
#include "lzw.h"

Entry gTbl[TABLELENGTH];

void lzwCompress(FILE* fs)
{
    unsigned int prev = fgetc(fs);
    unsigned int writeIdx = ENDFLAG;
    unsigned int idx;
    char c;
    initTbl(gTbl);
    output(CLEARFLAG);
    while(1)
    {
        while(1){
            if(feof(fs)){
                output(ENDFLAG);
                return;
            }
            c = fgetc(fs);
            idx = searchTbl(gTbl, prev, c);
            if(idx != INVALIDLENGTH)
                prev = idx;
            else
                break;
        }
        output(prev);
        writeIdx++;
        if(writeIdx == TABLELENGTH)
        {
            initTbl(gTbl);
            writeIdx = ENDFLAG + 1;
        }else
            writeTbl(gTbl, writeIdx, prev, c);
        prev = c;
    }
}

int main()
{
    FILE* fp = fopen("test.txt", "r");
    lzwCompress(fp);
    fclose(fp);
    return 0;
}
