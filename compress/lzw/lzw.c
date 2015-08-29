#include <stdio.h>
#include <ctype.h>
#include "lzw.h"

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

void showTblEntry(Entry *pTbl, unsigned int prev, char c)
{
    if (prev < 256) {
        LOG(" %2x %c", prev, isprint(prev)? prev: '.');
    } else {
        showTblEntry(pTbl, pTbl[prev].prev, pTbl[prev].c);
    }
    LOG(" %2x %c", c, isprint(c)? c: '.');
}

void writeTbl(Entry* pTbl, unsigned int idx, unsigned int prev, char c)
{
    Entry *p = pTbl + idx;
    setEntry(p, USEDFLAG, prev, c);
#ifdef _TEST_MODE_
    LOG("# set idx=%#04x", idx);
    showTblEntry(pTbl, prev, c);
    LOG("\n");
#endif
}

void outputCom(unsigned int i)
{
    printf("%c%c", (i>>8) & 0xff, i & 0xff);
}

void outputUncom(Entry *pTbl, unsigned int i)
{
    if (i < 256) {
        putchar(i);
    } else {
        outputUncom(pTbl, pTbl[i].prev);
        putchar(pTbl[i].c);
    }
}
