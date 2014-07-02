#include <stdio.h>

typedef struct
{
    char used;
    unsigned int prev;
    char c;
}Entry;

#define USEDFLAG                1
#define UNUSEFLAG               0

#define ORIGINLENGTH            8
#define CLEARFLAG               (2^ORIGINLENGTH)
#define ENDFLAG                 (CLEARFLAG+1)
#define TABLELENGTH             4096
#define INVALIDLENGTH           TABLELENGTH


void initTbl(Entry* pTbl);

int searchTbl(Entry* pTbl, unsigned int prev, char c);

void writeTbl(Entry* pTbl, unsigned int idx, unsigned int prev, char c);

void output(unsigned int i);
