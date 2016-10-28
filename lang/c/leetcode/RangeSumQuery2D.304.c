#include "leet.h"

struct NumMatrix {
    int **mat;
    int row;
    int col;
    int **acc;
};

void accumulate(struct NumMatrix *sln) {
    sln->acc = malloc(sizeof(int *) * (sln->row));
    int i, j;
    for (i = 0; i != sln->row; i++) {
        sln->acc[i] = malloc(sizeof(int) * sln->col);
        memcpy(sln->acc[i], sln->mat[i], sizeof(int) * sln->col);
    }
    for (j = 0; j != sln->row; j++) {
        for (i = 1; i != sln->col; i++) {
            sln->acc[j][i] += sln->acc[j][i - 1];
        }
    }
    for (j = 1; j != sln->row; j++) {
        for (i = 0; i != sln->col; i++) {
            sln->acc[j][i] += sln->acc[j - 1][i];
        }
    }
}

/** Initialize your data structure here. */
struct NumMatrix *NumMatrixCreate(int **matrix, int matrixRowSize, int matrixColSize) {
    struct NumMatrix *sln = malloc(sizeof(*sln));
    sln->mat = matrix;
    sln->row = matrixRowSize;
    sln->col = matrixColSize;
    sln->acc = NULL;
    if (matrixRowSize <= 0 || matrixColSize <= 0 || matrix == NULL) {
        return sln;
    }
    accumulate(sln);
    return sln;
}

int sumRegion(struct NumMatrix *sln, int row1, int col1, int row2, int col2) {
    if (row1 > row2 || col1 > col2 ||
        row1 < 0 || col1 < 0 ||
        row2 > sln->row || col2 > sln->col ||
        sln->acc == NULL) {
        return 0;
    }
    int a, b, c, d;
    a = sln->acc[row2][col2];
    b = row1 != 0 ? sln->acc[row1 - 1][col2] : 0;
    c = col1 != 0 ? sln->acc[row2][col1 - 1] : 0;
    d = row1 != 0 && col1 != 0 ? sln->acc[row1 - 1][col1 - 1] : 0;
    return a - b - c + d;
}

/** Deallocates memory previously allocated for the data structure. */
void NumMatrixFree(struct NumMatrix *sln) {
    int i;
    for (i = 0; i != sln->row; i++) {
        free(sln->acc[i]);
    }
    free(sln->acc);
    free(sln);
}

void NumMatrixShow(struct NumMatrix *sln) {
    printf("p=%p mat=%p row=%d col=%d acc=%p\n", sln, sln->mat, sln->row, sln->col, sln->acc);
    showArr2(sln->mat, sln->row, sln->col);
    showArr2(sln->acc, sln->row, sln->col);
}

#define EXPECT(a, b) {if (a != b) {printf("%s():%d\t%d != %d\n", \
                                          __func__, __LINE__, a, b); }}
void try() {
    int mat0[] = {3, 0, 1, 4, 2};
    int mat1[] = {5, 6, 3, 2, 1};
    int mat2[] = {1, 2, 0, 1, 5};
    int mat3[] = {4, 1, 0, 1, 7};
    int mat4[] = {1, 0, 3, 0, 5};
    int mat5[] = {1, 0, 3, 0, 5};
    int *mat[6] = {mat0, mat1, mat2, mat3, mat4, mat5};
    int row = 6;
    int col = 5;
    struct NumMatrix *sln = NumMatrixCreate(mat, row, col);
    int v = sumRegion(sln, 0, 0, 0, 0);
    EXPECT(v, 3);
    v = sumRegion(sln, 0, 0, 0, 2);
    EXPECT(v, 4);
    v = sumRegion(sln, 0, 0, 0, 4);
    EXPECT(v, 10);
    v = sumRegion(sln, 0, 1, 0, 4);
    EXPECT(v, 7);
    v = sumRegion(sln, 1, 1, 2, 2);
    EXPECT(v, 11);
    v = sumRegion(sln, 2, 1, 4, 3);
    EXPECT(v, 8);
    v = sumRegion(sln, 1, 2, 2, 4);
    EXPECT(v, 12);

    NumMatrixFree(sln);
}

void tryNull() {
    int **mat = NULL;
    struct NumMatrix *sln = NumMatrixCreate(mat, 0, 0);
    printf("return sln=%p\n", sln);
    int v = sumRegion(sln, 0, 0, 0, 0);
    printf("v=%d\n", v);
    NumMatrixFree(sln);
}

int main() {
    try();
    tryNull();
    return 0;
}
