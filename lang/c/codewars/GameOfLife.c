#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char *htmlize(int **, int, int);

int **alloc_grid(int row, int col) {
    printf("alloc row=%d col=%d\n", row, col);
    int **r = malloc(sizeof(int *) * row);
    int i;
    for (i = 0; i != row; i++) {
        r[i] = malloc(sizeof(int) * col);
    }
    return r;
}

int getVal(int **cells, int row, int col, int x, int y) {
    return (x >= 0 && x < row && y >= 0 && y < col) ? cells[x][y] : 0;
}

int convKer(int **cells, int row, int col, int x, int y) {
    return getVal(cells, row, col, x - 1, y - 1) +
           getVal(cells, row, col, x - 1, y) +
           getVal(cells, row, col, x - 1, y + 1) +
           getVal(cells, row, col, x, y - 1) +
           getVal(cells, row, col, x, y + 1) +
           getVal(cells, row, col, x + 1, y - 1) +
           getVal(cells, row, col, x + 1, y) +
           getVal(cells, row, col, x + 1, y + 1);
}

// calc from original value with currnet sum lived cell
int rule(int o, int s) {
    if (s < 2 || s > 3) {
        return 0;
    } else if (s == 3 && o == 0){
        return 1; // turn live from dead cell when negibour is 3
    } else {
        return o;
    }
}

void iter(int **cells, int generations, int row, int col, int **r) {
    int i, j, s;
    for (i = 0; i != row + 2; i++) {
        for (j = 0; j != col + 2; j++) {
            s = convKer(cells, row, col, i - 1, j - 1);
            r[i][j] = rule(getVal(cells, row, col, i - 1, j - 1), s);
        }
    }
}

bool allDeadCol(int **cells, int row, int colIdx) {
    int i;
    for (i = 0; i != row; i++) {
        if (cells[i][colIdx]) {
            return false;
        }
    }
    return true;
}
bool allDeadRow(int **cells, int rowIdx, int col) {
    int *row = cells[rowIdx];
    int i;
    for (i = 0; i != col; i ++) {
        if (row[i]) {
            return false;;
        }
    }
    return true;
}
int **contract(int **cells, int *rowptr, int *colptr) {
    // unlimited verison, grid will contract as need
    while (allDeadCol(cells, *rowptr, *colptr - 1)) {
        // printf("clean last col\n");
        (*colptr)--;
    }
    while (allDeadRow(cells, *rowptr - 1, *colptr)) {
        // printf("clean last row\n");
        free(cells[*rowptr - 1]);
        cells[*rowptr - 1] = NULL;
        (*rowptr)--;
    }
    while (allDeadCol(cells, *rowptr, 0)) {
        // printf("clean first col\n");
        (*colptr)--;
        int i, j;
        for (i = 0; i != *rowptr; i++) {
            for (j = 0; j != *colptr; j++) {
                cells[i][j] = cells[i][j + 1];
            }
        }
    }
    while (allDeadRow(cells, 0, *colptr)) {
        // printf("clean first row\n");
        free(cells[0]);
        (*rowptr)--;
        int i;
        for (i = 0; i != *rowptr; i++) {
            cells[i] = cells[i + 1];
        }
    }
    return cells;
}
void show_grid(int **cells, int row, int col) {
    int i, j;
    for (i = 0; i != row; i ++) {
        for (j= 0; j != col; j ++) {
            if (cells[i][j] == 2) {
                putchar('X');
            } else if (cells[i][j]) {
                putchar('*');
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
    printf("--------------\n");
}

int **get_generation(int **cells, int generations, int *rowptr, int *colptr) {
    // always allocate two rows/cols to support expanding/contracting
    int **r = alloc_grid(*rowptr + 2, *colptr + 2);

    iter(cells,  generations, *rowptr, *colptr, r);

    *rowptr += 2;
    *colptr += 2;
    // show_grid(r, *rowptr, *colptr);
    r = contract(r, rowptr, colptr);
    return r;
}

int eq_grid(int **cells, int row, int col, int **expect, int rowexp, int colexp) {
    if (row != rowexp || col != colexp) {
        printf("row %d!=%d col %d!=%d\n", row, rowexp, col, colexp);
        return -1;
    }
    int i, j;
    for (i = 0; i != row; i++) {
        for (j = 0; j != col; j++) {
            if (cells[i][j] != expect[i][j]) {
                printf("i=%d j=%d cells=%d != expect=%d\n", i, j, cells[i][j], expect[i][j]);
                return -1;
            }
        }
    }
    return 0;
}

int loop() {
    int a0[] = {1, 0, 0};
    int a1[] = {0, 1, 1};
    int a2[] = {1, 1, 0};
    int *cells[] = {a0, a1, a2};
    int row = 3, col = 3;
    show_grid(cells, row, col);
    int **r = cells;
    int i;
    for (i = 0; i != 5; i++) {
        r = get_generation(r, i, &row, &col );
        show_grid(r, row, col);
    //     int i;
    //     for (i = 0; i != row; i ++) {
    //         free(r[i]);
    //     }
    //     free(r);
    }
    return 0;
}

int test() {
    int a0[] = {1, 0, 0};
    int a1[] = {0, 1, 1};
    int a2[] = {1, 1, 0};
    int *cells[] = {a0, a1, a2};
    int row = 3, col = 3;
    show_grid(cells, row, col);
    int **r = get_generation(cells, 0, &row, &col );
    show_grid(r, row, col);
    int ret = eq_grid(cells, 3, 3, r, row, col);
    int i;
    for (i = 0; i != row; i ++) {
        free(r[i]);
    }
    free(r);
    return ret;
}


int main() {
    // test();
    loop();
    return 0;
}
