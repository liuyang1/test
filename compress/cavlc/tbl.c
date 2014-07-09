#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TOKENLEN            2
#define TOKENSIZELEN        1
#define TOKENSIZE           (TOKENLEN + TOKENSIZELEN)

#define TRAILINGSONES_MAX   4
#define TOTALCOEFF_MAX      17

char NC0_CoeffTokenTbl[TOTALCOEFF_MAX * TRAILINGSONES_MAX * TOKENSIZE] =
{
    1,      0b10000000,     0,
    0,      0,              0,
    0,      0,              0,
    0,      0,              0,

    6,      0b00010100,     0,
    2,      0b01000000,     0,
    0,      0,              0,
    0,      0,              0,

    8,      0b00000111,     0,
    6,      0b00010000,     0,
    3,      0b00100000,     0,
    0,      0,              0,

    9,      0b00000011,     0b10000000,
    8,      0b00000110,     0,
    7,      0b00001010,     0,
    5,      0b00011000,     0,

    10,     0b00000001,     0b11000000,
    9,      0b00000011,     0b00000000,
    8,      0b00000101,     0,
    6,      0b00001100,     0,

    11,     0b00000000,     0b11100000,
    10,     0b00000001,     0b10000000,
    9,      0b00000010,     0b10000000,
    7,      0b00001000,     0,

    13,     0,              0b01111000,
    11,     0,              0b11000000,
    10,     0b00000001,     0b01000000,
    8,      0b00000100,     0,
};

char*checkNC2Tbl(int nc)
{
    if (nc < -2) {
        printf("error, nc < -2\n");
        nc = -2;
    } else if (nc > 8)
        nc = 8;
    char* tbl[11] = {
        NULL,                                   // NC = -2
        NULL,                                   // NC = -1
        NC0_CoeffTokenTbl, NC0_CoeffTokenTbl,   // 0 <= NC < 2
        NULL, NULL,                             // 2 <= NC < 4
        NULL, NULL, NULL, NULL,                 // 4 <= NC < 8
        NULL                                    // 8 <= NC
    };
    return tbl[nc + 2];
}

int twoDim2One(int x, int y, int row)
{
    return x + y * row;
}

int writeCoeffToken(char* buf, int nc, int trailingones, int totalcoeff)
{
    char* tbl = checkNC2Tbl(nc);
    int tbloffset = twoDim2One(trailingones, totalcoeff, TRAILINGSONES_MAX);
    tbloffset = twoDim2One(0, tbloffset, TOKENSIZE);
    tbl += tbloffset;
    int bitoffset = *tbl;
    memcpy(buf, tbl + TOKENSIZELEN, TOKENLEN);
    return bitoffset;
}
