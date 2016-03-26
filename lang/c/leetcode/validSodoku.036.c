#include "leet.h"

#define SIZE 9
void showGroup(char **group, int size) {
    int i;
    for (i = 0; i != size; i++) {
        printf("%c ", *group[i]);
    }
    printf("\n");
}

bool isValidGroup(char **group, int size) {
    static bool a[SIZE];
    memset(a, 0, sizeof(bool) * SIZE);
    int i;
    char v;
    int idx;
    for (i = 0; i != SIZE; i++) {
        v = *group[i];
        if (v == '.') {
            continue;
        }
        idx = v - '1';
        if (a[idx]) {
            return false;
        }
        a[idx] = true;
    }
    return true;
}

bool isValidSudoku(char **board, int boardRowSize, int boardColSize) {
    if (boardRowSize != SIZE || boardColSize != SIZE) {
        printf("invalid size %dx%d\n", boardRowSize, boardColSize);
        return false;
    }
    char *group[SIZE];
    int i, j, u, v;
    for (i = 0; i != boardRowSize; i++) {
        for (j = 0; j != boardColSize; j++) {
            group[j] = board[i] + j;
        }
        if (!isValidGroup(group, SIZE)) {
            return false;
        }
    }
    for (j = 0; j != boardColSize; j++) {
        for (i = 0; i != boardRowSize; i++) {
            group[i] = board[i] + j;
        }
        if (!isValidGroup(group, SIZE)) {
            return false;
        }
    }
    for (i = 0; i != SIZE / 3; i++) {
        for (j = 0; j != SIZE / 3; j++) {
            for (u = 0; u != 3; u++) {
                for (v = 0; v != 3; v++) {
                    group[u * 3 + v] = board[i * 3 + u] + j * 3 + v;
                }
            }
            if (!isValidGroup(group, SIZE)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    char *board[] = {".87654321", "......2..", "3........", "4........",
                     "5........", "6........", "7........", "8........",
                     "9........"};
    bool r = isValidSudoku(board, 9, 9);
    printf("r=%s\n", SBOOL(r));
    return true;
}
