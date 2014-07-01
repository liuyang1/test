#include <stdio.h>

typedef struct
{
    char used;
    unsigned int prev;
    char c;
}Entry;

#define USEDFLAG                1
#define UNUSEFLAG               0

void setEntry(Entry* p, int used, unsigned int prev, char c)
{
    p->used = used;
    p->prev = prev;
    p->c = c;
}

int checkEntry(Entry* pEntry, unsigned int prev, char c)
{
    return pEntry->used == USEDFLAG && pEntry->prev == prev && pEntry->c == c;
}

void dispEntry(Entry* pEntry)
{
    printf("entry: %d %d %c\n", pEntry->used, pEntry->prev, pEntry->c);
}

#define BEGINENCODING           256
#define ENDENCODING             257
#define TABLELENGTH             4096
#define INVALIDLENGTH           TABLELENGTH

Entry gTbl[TABLELENGTH];

void initTbl(Entry* pTbl)
{
    for(int i=0; i < ENDENCODING; i++, pTbl++)
        setEntry(pTbl, USEDFLAG, INVALIDLENGTH, (char)i);
    for(int i=ENDENCODING; i < TABLELENGTH; i++, pTbl++)
        setEntry(pTbl, UNUSEFLAG, INVALIDLENGTH, 0);
}

int searchTbl(Entry* pTbl, unsigned int prev, char c)
{
    for(int i=0; i < INVALIDLENGTH; i++)
        if(checkEntry(pTbl + i, prev, c))
            return i;
    return INVALIDLENGTH;
}

void writeTbl(Entry* pTbl, unsigned int idx, unsigned int prev, char c)
{
    Entry *p = pTbl + idx;
    setEntry(p, USEDFLAG, prev, c);
}

void output(unsigned int i)
{
    // printf("%d\n", i);
    printf("%c%c", (i>>8) & 0xff, i & 0xff);
    //printf("%d %x %x\n", i, (i>>8) & 0xff, i & 0xff);
}

void lzwCompress(FILE* fs)
{
    unsigned int prev = fgetc(fs);
    unsigned int writeIdx = ENDENCODING;
    unsigned int idx;
    char c;
    initTbl(gTbl);
    output(BEGINENCODING);
    while(1)
    {
        while(1){
            if(feof(fs)){
                output(ENDENCODING);
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
            writeIdx = ENDENCODING + 1;
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
