#include "leet.h"

typedef struct {
    bool *arr;
    int len;
    int cap;
} LimitSet;

LimitSet *lset_create(int n) {
    LimitSet *lset = malloc(sizeof(LimitSet));
    lset->cap = n;
    lset->arr = malloc(sizeof(bool) * lset->cap);
    int i;
    for (i = 0; i != lset->cap; i++) {
        lset->arr[i] = true;
    }
    lset->len = lset->cap;
    return lset;
}

void lset_destroy(LimitSet *lset) {
    free(lset->arr);
    free(lset);
}

/**
 * @brief iterator style interface
 * input current idx, return to next permit index
 */
int lset_next(LimitSet *lset, int idx) {
    for (idx++; idx < lset->cap; idx++) {
        if (lset->arr[idx] == true) {
            return idx;
        }
    }
    return -1;
}

int lset_get(LimitSet *lset, int idx) {
    if (lset->arr[idx]) {
        lset->arr[idx] = false;
        lset->len--;
        return idx;
    }
    // cannot return
    return -1;
}

void lset_put(LimitSet *lset, int idx) {
    if (!lset->arr[idx]) {
        lset->arr[idx] = true;
        lset->len++;
    }
    // cannot put
}

int lset_size(LimitSet *lset) {
    return lset->len;
}

int isAttack(int *chosen, int m, int v) {
    int i, d, e;
    for (i = 0; i != m; i++) {
        d = v - chosen[i];
        e = m - i;
        if (d == e || d == -e) {
            return true;
        }
    }
    return false;
}

void strBoard(char ***board, int *chosen, int n) {
    char **r = malloc(sizeof(char *) * n);
    int i, j;
    for (i = 0; i != n; i++) {
        r[i] = malloc(sizeof(char) * (n + 1));
        for (j = 0; j != n; j++) {
            r[i][j] = i == chosen[j] ? 'Q' : '.';
        }
        r[i][j] = '\0';
    }
    *board = r;
}

/**
 * @brief recurse to search queen soltuion
 *
 * @param r          OUT arr of queen string arr, size is (N + 1) * N * returnSize
 * @param returnSize OUT solution number
 * @param n          IN  n queen
 * @param chosen     IN  already selected position
 *                       When recurse back, chosen is not change, so only IN
 * @param m          IN  already selected position counter
 * @param cand       IN  candidate position set
 */
void queenRecr(char ****r, int *returnSize, int n, int *chosen, int m,
               LimitSet *cand) {
    if (lset_size(cand) == 0) {
        (*returnSize)++;
        reallocM(r, sizeof(void *) * (*returnSize));
        // - r, returnSize is OUT, so need Dereference op.
        // - strBoard also need OUT, so add Address-of op.
        strBoard(&((*r)[*returnSize - 1]), chosen, n);
    }
    // select one position from candicates
    // if not attch previous selected position,
    //      then remove it from candidate recu to next position
    // until all candicates checked
    int i = -1;
    for (i = lset_next(cand, i); i != -1; i = lset_next(cand, i)) {
        if (!isAttack(chosen, m, i)) {
            chosen[m] = lset_get(cand, i);
            queenRecr(r, returnSize, n, chosen, m + 1, cand);
            lset_put(cand, i);
        }
    }
}

char ***solveNQueens(int n, int *returnSize) {
    char ***r = NULL;
    *returnSize = 0;
    int *queen = malloc(sizeof(int) * n);
    memset(queen, sizeof(int) * n, 0);
    LimitSet *cand = lset_create(n);

    queenRecr(&r, returnSize, n, queen, 0, cand);

    lset_destroy(cand);
    free(queen);
    return r;
}

int main() {
    int n = 5;
    int size;
    char ***r = solveNQueens(n, &size);
    printf("queen(%d) = %d\n", n, size);

    int i, j;
    for (i = 0; i != size; i++) {
        for (j = 0; j != n; j++) {
            printf("%s\n", r[i][j]);
        }
        putchar('\n');
    }

    for (i = 0; i != size; i++) {
        for (j = 0; j != n; j++) {
            free(r[i][j]);
        }
        free(r[i]);
    }
    free(r);
    return 0;
}
