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

    13,     0,              0b01011000,
    13,     0,              0b01110000,
    11,     0,              0b10100000,
    9,      0b00000010,     0,

    13,     0,              0b01000000,
    13,     0,              0b01010000,
    13,     0,              0b01101000,
    10,     0b00000001,     0,

    14,     0,              0b00111100,
    14,     0,              0b00111000,
    13,     0,              0b01001000,
    11,     0,              0b10000000,

    14,     0,              0b00101100,
    14,     0,              0b00101000,
    14,     0,              0b00110100,
    13,     0,              0b01100000,

    15,     0,              0b00011110,
    15,     0,              0b00011100,
    14,     0,              0b00100100,
    14,     0,              0b00110000,

    15,     0,              0b00010110,
    15,     0,              0b00010100,
    15,     0,              0b00011010,
    14,     0,              0b00100000,

    16,     0,              0b00001111,
    15,     0,              0b00000010,
    15,     0,              0b00010010,
    15,     0,              0b00011000,

    16,     0,              0b00001011,
    16,     0,              0b00001110,
    16,     0,              0b00001101,
    15,     0,              0b00010000,

    16,     0,              0b00000111,
    16,     0,              0b00001010,
    16,     0,              0b00001001,
    16,     0,              0b00001100,

    16,     0,              0b00000100,
    16,     0,              0b00000110,
    16,     0,              0b00000101,
    16,     0,              0b00001000,
};

char NC2_CoeffTokenTbl[TOTALCOEFF_MAX * TRAILINGSONES_MAX * TOKENSIZE] =
{
};

char NC4_CoeffTokenTbl[TOTALCOEFF_MAX * TRAILINGSONES_MAX * TOKENSIZE] =
{
};

char NC8_CoeffTokenTbl[TOTALCOEFF_MAX * TRAILINGSONES_MAX * TOKENSIZE] =
{
};

char NC_1_CoeffTokenTbl[TOTALCOEFF_MAX * TRAILINGSONES_MAX * TOKENSIZE] =
{
};

char NC_2_CoeffTokenTbl[TOTALCOEFF_MAX * TRAILINGSONES_MAX * TOKENSIZE] =
{
};

char*checkNC2Tbl(int nc)
{
    if (nc < -2) {
        printf("error, nc < -2\n");
        nc = -2;
    } else if (nc > 8)
        nc = 8;
    char* tbl[11] = {
        NC_2_CoeffTokenTbl,                                     // NC = -2
        NC_1_CoeffTokenTbl,                                     // NC = -1
        NC0_CoeffTokenTbl, NC0_CoeffTokenTbl,                   // 0 <= NC < 2
        NC2_CoeffTokenTbl, NC2_CoeffTokenTbl,                   // 2 <= NC < 4
        // 4 <= NC < 8
        NC4_CoeffTokenTbl, NC4_CoeffTokenTbl, NC4_CoeffTokenTbl, NC4_CoeffTokenTbl,
        NC8_CoeffTokenTbl                                       // 8 <= NC
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
