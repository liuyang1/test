#include <stdio.h>
// totalCoeff, trailingOnes, data
int NC0_CoeffTokenTbl[17][4][17]=
{
    {
        {1,     1},
        {0},
        {0},
        {0},
    },{
        {6,     0, 0, 0, 1,    0, 1},
        {2,     0, 1},
        {0},
        {0}
    },{
        {8,     0, 0, 0, 0,    0, 1, 1, 1},
        {6,     0, 0, 0, 1,    0, 0},
        {3,     0, 0, 1},
        {0}
    },{
        {9,     0, 0, 0, 0,    0, 0, 1, 1,      1},
        {8,     0, 0, 0, 0,    0, 1, 1, 0},
        {7,     0, 0, 0, 0,    1, 0, 1},
        {5,     0, 0, 0, 1,    1},
    },{
        {10,    0, 0, 0, 0,     0, 0, 0, 1,     1, 1},
        {9,     0, 0, 0, 0,     0, 0, 1, 1,     0},
        {8,     0, 0, 0, 0,     0, 1, 0, 1},
        {6,     0, 0, 0, 0,     1, 1},
    },{
        {11,    0, 0, 0, 0,     0, 0, 0, 0,     1, 1, 1},
        {10,    0, 0, 0, 0,     0, 0, 0, 1,     1, 0},
        {9,     0, 0, 0, 0,     0, 0, 1, 0,     1},
        {7,     0, 0, 0, 0,     0, 1, 0}
    }
};

//int[4][17][] checkNC2Tbl(int nc)
int checkNC2Tbl(int nc)
{
    int offset = -2;
    int** tbl[11] ={
        NULL,                                   // NC = -2
        NULL,                                   // NC = -1
        NC0_CoeffTokenTbl, NC0_CoeffTokenTbl,   // 0 <= NC < 2
        NULL, NULL,                             // 2 <= NC < 4
        NULL, NULL, NULL, NULL,                 // 4 <= NC < 8
        NULL                                    // 8 <= NC
    };
    if(nc<-2){
        printf("error, nc < -2\n");
    }else if(nc > 8)
        nc = 8;
    int[17][4][17]* ptr = NC0_CoeffTokenTbl;
    // return tbl[nc+offset];
    return 0;
}

