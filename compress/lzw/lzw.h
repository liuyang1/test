#ifndef _LZW_H_
#define _LZW_H_

typedef struct {
    char used;
    unsigned int prev;
    char c;
} Entry;

#define USEDFLAG                1
#define UNUSEFLAG               0

#define ORIGINLENGTH            8
#define CLEARFLAG               (1 << ORIGINLENGTH)
#define ENDFLAG                 (CLEARFLAG)
#define TABLELENGTH             4096
#define INVALIDLENGTH           TABLELENGTH


#define _TEST_MODE_

#ifdef _TEST_MODE_
#define LOG(...)        fprintf(stderr, __VA_ARGS__);
#else
#define LOG(...)
#endif

void initTbl(Entry *pTbl);

int searchTbl(Entry *pTbl, unsigned int prev, char c);

void writeTbl(Entry *pTbl, unsigned int idx, unsigned int prev, char c);

void outputCom(unsigned int i);

void outputUncom(Entry *pTbl, unsigned int i);
void showTblEntry(Entry *pTbl, unsigned int prev, char c);

#endif
