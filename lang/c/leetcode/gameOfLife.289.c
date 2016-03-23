#include "leet.h"

int isLive(int **board, int boardRowSize, int boardColSize,
           int x, int y) {
    if (x < 0 || x >= boardRowSize || y < 0 || y >= boardColSize) {
        return 0;
    }
    return board[x][y];
}

int sumNeighbors(int **board, int boardRowSize, int boardColSize,
                 int x, int y) {
    int i, j, sum = 0;
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            sum += isLive(board, boardRowSize, boardColSize, x + i, y + j);
        }
    }
    return sum;
}

int liveOrDie(bool live, int neighbors) {
    if (neighbors < 2) {
        return false;
    } else if (neighbors > 3) {
        return false;
    } else if (live && neighbors >= 2 && neighbors <= 3) {
        return true;
    } else if (neighbors == 3) {
        return true;
    }
    // When neighbors == 2, and dead, fallback to this case
    return false;
}

void gameOfLife(int **board, int boardRowSize, int boardColSize) {
    int *t0 = malloc(sizeof(int) * boardColSize);
    int *t1 = malloc(sizeof(int) * boardColSize);
    int *curr, *prev;
    int i, j, sum;
    for (i = 0; i != boardRowSize; i++) {
        if (i % 2 == 0) {
            curr = t0;
            prev = t1;
        } else {
            curr = t1;
            prev = t0;
        }
        for (j = 0; j != boardColSize; j++) {
            sum = sumNeighbors(board, boardRowSize, boardColSize, i, j);
            curr[j] = liveOrDie(board[i][j], sum);
        }
        // delay one line then overwrite it.
        if (i >= 1) {
            for (j = 0; j != boardColSize; j++) {
                board[i - 1][j] = prev[j];
            }
        }
    }
    for (j = 0; j != boardColSize; j++) {
        board[i - 1][j] = curr[j];
    }
    free(t0);
    free(t1);
}

int main() {
    int a[][3] = {{1, 0, 0}, {0, 0, 1}, {1, 0, 1}};
    int **aa = malloc(sizeof(int *) * 3);
    aa[0] = a[0];
    aa[1] = a[1];
    aa[2] = a[2];
    gameOfLife(aa, 3, 3);
    int row = 3, i;
    for (i = 0; i != row; i++) {
        showArr(aa[i], 3);
    }
    free(aa);
    return 0;
}
