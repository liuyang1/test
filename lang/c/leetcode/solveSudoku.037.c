/*
 * Sudoku problem
 * Ref: https://leetcode.com/problems/sudoku-solver/
 *
 * Board format: C-string array, char '.' means empty cell.
 *               char '1' .. '9' means corrspoding number.
 *
 * This solution only find ONE valid solution.
 */
#include "leet.h"

#define SIZE 9

#define returnIfFalse(b) {if (!(b)) {return false; }}

void showCand(bool *a) {
    int i;
    printf("cand: ");
    for (i = 0; i != SIZE; i++) {
        if (a[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void showBoard(char **board) {
    int i;
    for (i = 0; i != SIZE; i++) {
        printf("%s\n", board[i]);
    }
    printf("\n");
}

char **boardDup(char **board) {
    int i;
    char **newBoard = malloc(sizeof(char *) * SIZE);
    for (i = 0; i != SIZE; i++) {
        newBoard[i] = strdup(board[i]);
    }
    return newBoard;
}

void boardFree(char **board) {
    int i;
    for (i = 0; i != SIZE; i++) {
        free(board[i]);
    }
    free(board);
}

void boardCopy(char **dst, char **src) {
    int i;
    for (i = 0; i != SIZE; i++) {
        strcpy(dst[i], src[i]);
    }
}

bool isUndeterm(char **board, int i, int j) {
    return board[i][j] == '.';
}

void boardDeterm(char **board, int i, int j, int v) {
    board[i][j] = '1' + v;
}

void boardUndterm(char **board, int i, int j) {
    board[i][j] = '.';
}

bool filterCand(char **group, bool *a, int *num) {
    int i, idx;
    char v;
    for (i = 0; i != 9; i++) {
        v = *group[i];
        if (v == '.') {
            continue;
        }
        idx = v - '1';
        if (a[idx]) {
            a[idx] = false;
            *num = *num - 1;
        }
    }
    return true;
}

/**
 * @brief filterCons
 * for cell (x,y) in board, check row, col, block 3 constraints,
 *      return remain candidates for cell (x, y)
 *
 * @param board
 * @param x
 * @param y
 * @param a     return candidates array,
 *              a[i] = true, means fill (i + 1) at this cell is possible
 * @param num   return numbers of candidates
 *
 * @return if no exist candidates, return false, else true
 */
bool filterCons(char **board, int x, int y,
                bool *a, int *num)
{
    char *group[SIZE];
    memset(a, 1, sizeof(bool) * SIZE);
    int i, j;
    bool r;
    // row constraint
    for (j = 0; j != SIZE; j++) {
        group[j] = board[x] + j;
    }
    returnIfFalse(r = filterCand(group, a, num));
    // col constraint
    for (i = 0; i != SIZE; i++) {
        group[i] = board[i] + y;
    }
    returnIfFalse(r = filterCand(group, a, num));
    // block constraint
    for (i = 0; i != 3; i++) {
        for (j = 0; j != 3; j++) {
            // modular divide 3, to get its block
            group[i * 3 + j] = board[(x / 3) * 3 + i] + (y / 3) * 3 + j;
        }
    }
    returnIfFalse(r = filterCand(group, a, num));
    return r;
}

int findOneCand(bool *a) {
    int i;
    for (i = 0; i != SIZE; i++) {
        if (a[i]) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief continue to find cell which have only ONE candidates,
 *              fill it, until cannot find one
 *
 * @param board
 *
 * @return number of filled cells
 * if find one cell which have NO candidates,
 *      this mean it's imposibble for this branch, return -1
 */
int directInfer(char **board) {
    int precnt = 0, cnt = 0;
    int i, j;
    while (1) {
        cnt = 0;
        for (i = 0; i != SIZE; i++) {
            for (j = 0; j != SIZE; j++) {
                if (!isUndeterm(board, i, j)) {
                    cnt++;
                    continue;
                }
                int num = SIZE;
                bool r, a[SIZE];
                r = filterCons(board, i, j, a, &num);
                if (num == 0) {
                    LOG("no valid for pos=(%d,%d)\n", i, j);
                    return -1;
                } else if (num == 1) {
                    // find the one, fill it.
                    int v = findOneCand(a);
                    boardDeterm(board, i, j, v);
                    cnt++;
                }
            }
        }
        if (precnt == cnt) {
            break;
        }
        precnt = cnt;
    }
    return cnt;
}

/**
 * @brief find the cell which have least number of candidates
 * This is for less guess branch, so quick find solution
 *
 * @param board
 * @param x         return the cell position with x and y
 * @param y
 * @param a         return candidates of the cell
 *
 * @return number of candidates
 */
int findLeastCand(char **board, int *x, int *y, bool *a) {
    int i, j, k;
    int leastnum = -1;
    *x = -1;
    *y = -1;
    for (i = 0; i != SIZE; i++) {
        for (j = 0; j != SIZE; j++) {
            if (!isUndeterm(board, i, j)) {
                continue;
            }
            int num = SIZE;
            bool r, ta[SIZE];
            r = filterCons(board, i, j, ta, &num);
            if (num <= 1) {
                LOG("should not here (%d,%d) cand num=%d\n", i, j, num);
                return -1;
            } else if (leastnum == -1 || leastnum > num) {
                *x = i;
                *y = j;
                leastnum = num;
                memcpy(a, ta, sizeof(bool) * SIZE);
            }
        }
    }
    LOG("(%d, %d) cand num=%d\n", *x, *y, leastnum);
    // showCand(a);
    return leastnum;
}

/**
 * @brief - fill all cell as many as possible
 *        - find the cell which have least candidates
 *        - guess one, then try continue to solve, if fail, guess next one
 *
 * `solveSudoku` function is API defined by leetcode.
 * so have boardRowSize, boardColSize args
 *
 * @return return filled cell number, if no solution, return -1
 */
int solveSudoku(char **board, int boardRowSize, int boardColSize) {
    int cnt = directInfer(board);
    if (cnt == SIZE * SIZE || cnt == -1) {
        return cnt;
    }
    bool a[SIZE];
    int x, y, num, i;
    num = findLeastCand(board, &x, &y, a);
    if (num == -1) {
        return -1;
    }
    for (i = 0; i != SIZE; i++) {
        if (a[i]) {
            LOG("guess (%d,%d)=%d\n", x, y, i);
            // have to duplicate board, then continue guess
            // as it's not easy to resume back, especially for directInfer
            char **newBoard = boardDup(board);
            boardDeterm(newBoard, x, y, i);
            cnt = solveSudoku(newBoard, boardRowSize, boardColSize);
            if (cnt == SIZE * SIZE) {
                boardCopy(board, newBoard);
                boardFree(newBoard);
                return cnt;
            }
            LOG("guess (%d,%d)=%d wrong\n", x, y, i);
            boardFree(newBoard);
        }
    }
    return -1;
}

int main() {
    char *input[] = {"..9748...","7........", ".2.1.9...", "..7...24.",
                     ".64.1.59.", ".98...3..",
                     "...8.3.2.", "........6", "...2759.."};
    char **board= boardDup(input);
    solveSudoku(board, SIZE, SIZE);
    showBoard(board);
    boardFree(board);
    return 0;
}
