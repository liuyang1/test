#include <stdio.h>
#include "lzw.h"
#define _TEST_MODE_

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

void initTbl(Entry* pTbl)
{
    for(int i=0; i < ENDFLAG; i++, pTbl++)
        setEntry(pTbl, USEDFLAG, INVALIDLENGTH, (char)i);
    for(int i=ENDFLAG; i < TABLELENGTH; i++, pTbl++)
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

void outputCom(unsigned int i)
{
#ifdef _TEST_MODE_
    printf("%3d 0x%02x%02x\n", i, (i>>8) & 0xff, i & 0xff);
#else
    printf("%c%c", (i>>8) & 0xff, i & 0xff);
#endif
}

void outputUncom(char c)
{
#ifdef _TEST_MODE_
    printf("%c\n", c);
#else
    putchar(c);
#endif
}
