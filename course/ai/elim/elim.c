#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define ROWS_BRD        (size_t)8u
#define COLS_BRD        (size_t)8u

#define VVV(...)

#define pm_enc_t        uint64_t

typedef struct {
    bool b[ROWS_BRD * COLS_BRD];
    size_t combo;
    size_t score;
} board_t;

typedef struct {
    size_t rows, cols;
    size_t nblk;
    bool *b;
    pm_enc_t enc;
#ifdef ENC1D
    pm_enc_t *enc1d;
#endif
} shape_t;

size_t g_shp_n;
shape_t **g_shp_lst;

#define brd_elm_set(b, i, j, x)     b->b[(i) * COLS_BRD + (j)] = (x)
#define brd_elm_get(b, i, j)        b->b[(i) * COLS_BRD + (j)]

void shp_show(const shape_t *const s, FILE *fp);

board_t *brd_open() {
    board_t *b = malloc(sizeof(board_t));
    size_t i, j;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            brd_elm_set(b, i, j, false);
        }
    }
    b->combo = 0;
    b->score = 0;
    return b;
};

void brd_close(board_t *b) {
    free(b);
}

board_t *brd_dup(const board_t *const b) {
    board_t *p = malloc(sizeof(board_t));
    *p = *b;
    return p;
}

void brd_show(const board_t *const b, FILE *fp) {
    fprintf(fp, "score=%4zu comb=%zu\n", b->score, b->combo);
    size_t i, j;
    fprintf(fp, "+");
    for (i = 0; i != ROWS_BRD * 2; i++) {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
    for (i = 0; i != ROWS_BRD; i++) {
        fprintf(fp, "|");
        for (j = 0; j != COLS_BRD; j++) {
            fprintf(fp, "%c ", brd_elm_get(b, i, j) ? 'O' : ' ');
        }
        fprintf(fp, "|");
        fprintf(fp, "\n");
    }
    fprintf(fp, "+");
    for (i = 0; i != ROWS_BRD * 2; i++) {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
}

int brd_score(board_t *b) {
    return 0;
}

/** do that put shape to board with (x, y) position
 * it must be check with brd_fit function */
void brd_put(board_t *const b, const shape_t *const s, size_t x, size_t y) {
    size_t i, j, k;
    for (i = k = 0; i != s->rows; i++) {
        for (j = 0; j != s->cols; j++, k++) {
            if (s->b[k] == false) {
                continue;
            }
            // else: s->b[k] == true
            if (brd_elm_get(b, x + i, y + j)) {
                assert(0); // must brd_fit
            }
            brd_elm_set(b, x + i, y + j, true);
        }
    }
    b->score += s->nblk;
}

void brd_unput(board_t *b, shape_t *s, size_t x, size_t y) {
    size_t i, j, k;
    for (i = k = 0; i != s->rows; i++) {
        for (j = 0; j != s->cols; j++, k++) {
            if (s->b[k] == false) {
                continue;
            }
            assert(brd_elm_get(b, x + i, y + j));
            brd_elm_set(b, x + i, y + j, false);
        }
    }
    b->score -= s->nblk;
}

/** check shape to board with (x, y) postion, fit or not */
static inline bool brd_fit(const board_t *const b, const shape_t *const s, size_t x, size_t y) {
    size_t i, j, k;
    if (x + s->rows > ROWS_BRD || y + s->cols > COLS_BRD) {
        VVV("out of range x=%zu y=%zu shape=%zu*%zu board=%zu*%zu\n",
            x, y, s->rows, s->cols, ROWS_BRD, COLS_BRD);
        return false;
    }
    for (i = k = 0; i != s->rows; i++) {
        for (j = 0; j != s->cols; j++, k++) {
            if (s->b[k] == false) {
                continue;
            }
            // else: s->b[k] == true
            if (brd_elm_get(b, x + i, y + j) == true) {
                VVV("conflict x=%zu y=%zu shape=%zu*%zu board=%zu*%zu at %zu*%zu\n",
                    x, y, s->rows, s->cols, ROWS_BRD, COLS_BRD, i, j);
                return false;
            }
        }
    }
    return true;
}

void brd_chk_line(board_t *b, bool *rows, bool *cols) {
    // simple loop, faster loop
    size_t i, j;
    for (i = 0; i != ROWS_BRD; i++) {
        rows[i] = true;
        for (j = 0; j != COLS_BRD; j++) {
            rows[i] = rows[i] && brd_elm_get(b, i, j);
        }
    }
    for (j = 0; j != COLS_BRD; j++) {
        cols[j] = true;
        for (i = 0; i != ROWS_BRD; i++) {
            cols[j] = cols[j] && brd_elm_get(b, i, j);
        }
    }
}

void brd_do_elim(board_t *b, bool *rows, bool *cols) {
    size_t i, j;
    for (i = 0; i != ROWS_BRD; i++) {
        if (rows[i]) {
            for (j = 0; j != COLS_BRD; j++) {
                brd_elm_set(b, i, j, false);
            }
        }
    }
    for (j = 0; j != COLS_BRD; j++) {
        if (cols[j]) {
            for (i = 0; i != ROWS_BRD; i++) {
                brd_elm_set(b, i, j, false);
            }
        }
    }
}

void brd_elim(board_t *b) {
    bool rows[ROWS_BRD], cols[COLS_BRD];
    size_t i;
    for (i = 0; i != ROWS_BRD; i++) {
        rows[i] = false;
    }
    for (i = 0; i != COLS_BRD; i++) {
        cols[i] = false;
    }
    brd_chk_line(b, rows, cols);
    size_t line_cnt = 0;
    for (i = 0; i != ROWS_BRD; i++) {
        line_cnt += rows[i];
    }
    for (i = 0; i != COLS_BRD; i++) {
        line_cnt += cols[i];
    }
    if (line_cnt != 0) {
        b->combo++;
    } else {
        b->combo = 0;
    }
    b->score += 10 * line_cnt * (line_cnt + 1) / 2; // 10, 30, 60
    b->score += 10 * b->combo * (b->combo + 1) / 2; // 10, 30, 60
    brd_do_elim(b, rows, cols);
}

size_t brd_try_elim(board_t *b) {
    bool rows[ROWS_BRD], cols[COLS_BRD];
    size_t i;
    for (i = 0; i != ROWS_BRD; i++) {
        rows[i] = false;
    }
    for (i = 0; i != COLS_BRD; i++) {
        cols[i] = false;
    }
    brd_chk_line(b, rows, cols);
    size_t line_cnt = 0, combo, score = b->score;
    for (i = 0; i != ROWS_BRD; i++) {
        line_cnt += rows[i];
    }
    for (i = 0; i != COLS_BRD; i++) {
        line_cnt += cols[i];
    }
    if (line_cnt != 0) {
        combo = b->combo + 1;
    } else {
        combo = 0;
    }
    score += 10 * line_cnt * (line_cnt + 1) / 2; // 10, 30, 60
    score += 10 * combo * (combo + 1) / 2; // 10, 30, 60
    // This is NOT real score. this is virtual for not full of a line
    if (line_cnt == 0) {
        size_t j, ml = 0, l;
        for (i = 0; i != ROWS_BRD; i++) {
            l = 0;
            for (j = 0; j != COLS_BRD; j++) {
                l += brd_elm_get(b, i, j);
            }
            if (l > ml) {
                ml = l;
            }
        }
        for (j = 0; j != COLS_BRD; j++) {
            l = 0;
            for (i = 0; i != ROWS_BRD; i++) {
                l += brd_elm_get(b, i, j);
            }
            if (l > ml) {
                ml = l;
            }
        }
        score += ml;
    }
    return score;
}

/************** BOARD FILL function *******************************************/
bool brd_fill(board_t *b, shape_t *shp);
bool nothing(board_t *b, shape_t *shp) {
    return false;
}

bool first_fit(board_t *b, shape_t *shp) {
    size_t i, j;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            if (brd_fit(b, shp, i, j)) {
                brd_put(b, shp, i, j); // put at first fit
                return true;
            }
        }
    }
    return false;
}

size_t fit_cnt_naive(const board_t *const b, const shape_t *const shp) {
    size_t i, j, cnt = 0;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            cnt += brd_fit(b, shp, i, j);
        }
    }
    return cnt;
}

// two-dim patter-matching with Rabin-Karp algo
size_t fit_cnt_enc(const board_t *const b, const shape_t *const shp) {
    size_t i, j, cnt = 0;
    for (i = 0; i != ROWS_BRD - shp->rows + 1; i++) {
        for (j = 0; j != COLS_BRD - shp->cols + 1; j++) {
            pm_enc_t enc;
            enc = 0;
            size_t u, v;
            for (u = 0; u != shp->rows; u++) {
                for (v = 0; v != shp->cols; v++) {
                    // enc = (enc << 1) + (brd_elm_get(b, i + u, j + v));
                    enc = (enc << 1) + (!brd_elm_get(b, i + u, j + v));
                }
            }
            // printf("%zux%zu enc=%lx\n", i, j, enc);
            // cnt += (enc | shp->enc) == shp->enc;
            cnt += (enc & shp->enc) == shp->enc;
        }
    }
    return cnt;
}

#if 0
/** This is better complexity, but wrose performance, due to shape size is tiny
 * This encoding optimization is bad */
size_t fit_cnt_RK(const board_t *const b, const shape_t *const shp) {
    size_t i, j, cnt = 0;
    for (i = 0; i != ROWS_BRD - shp->rows + 1; i++) {
        j = 0;
        size_t u, v = 0;
        pm_enc_t enc1d[ROWS_BRD] = {0};
        for (u = 0; u != shp->rows; u++) {
            for (v = 0; v != shp->cols; v++) {
                enc1d[u] = (enc1d[u] << 1) + (!brd_elm_get(b, i + u, j + v));
            }
        }
        for (; j != COLS_BRD - shp->cols + 1; j++) {
            bool fit = true;
            size_t k;
            for (k = 0; k != shp->rows; k++) {
                fit = fit && ((enc1d[k] & shp->enc1d[k]) == shp->enc1d[k]);
            }
            cnt += fit;

            for (u = 0; u != shp->rows; u++) {
                // enc1d[u] = ((enc1d[u] << 1) & ((1 << shp->cols) - 1))
                            // + (!brd_elm_get(b, i + u, j + v));
                enc1d[u] = (enc1d[u] << 1) + (!brd_elm_get(b, i + u, j + v));
            }
        }
    }
    return cnt;
}
#endif

size_t fit_cnt(const board_t *const b, const shape_t *const shp) {
#if 1
    // return fit_cnt_naive(b, shp);
    return fit_cnt_enc(b, shp);
    // return fit_cnt_RK(b, shp);
#else
    size_t x0 = fit_cnt_naive(b, shp);
    size_t x1 = fit_cnt_RK(b, shp);
    if (x0 == x1) {
        return x0;
    } else {
        brd_show(b, stderr);
        shp_show(shp, stderr);
        fprintf(stderr, "fit cnt: %zu %zu\n", x0, x1);
        assert(0);
    }
#endif
}

#if 0
bool corner(board_t *b, shape_t *shp) {
    size_t max_val = 0, mi, mj;
    size_t i, j;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            if (brd_fit(b, shp, i, j)) {
                size_t pos_val = abs(i - (shp->rows + ROWS_BRD - 1) / 2) +
                                 abs(j - (shp->cols + COLS_BRD - 1) / 2);
                if (pos_val >= max_val) {
                    max_val = pos_val;
                    mi = i, mj = j;
                }
            }
        }
    }
    if (max_val == 0) {
        return false;
    }
    brd_put(b, shp, mi, mj);
    return true;
}

bool center(board_t *b, shape_t *shp) {
    size_t max_val = 0, mi, mj;
    size_t i, j;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            if (brd_fit(b, shp, i, j)) {
                size_t pos_val = 0;
                if (i < (ROWS_BRD - 1) / 2) {
                    pos_val += i + 1;
                } else {
                    pos_val += ROWS_BRD - i;
                }
                if (j < (COLS_BRD - 1) / 2) {
                    pos_val += j + 1;
                } else {
                    pos_val += COLS_BRD - j;
                }
                assert(pos_val > 0);
                if (pos_val >= max_val) {
                    max_val = pos_val;
                    mi = i, mj = j;
                }
            }
        }
    }
    if (max_val == 0) {
        return false;
    }
    brd_put(b, shp, mi, mj);
    return true;
}

bool fill_random(board_t *b, shape_t *shp) {
    bool fit[ROWS_BRD][COLS_BRD];
    size_t i, j, cnt;
    for (i = cnt = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            cnt += fit[i][j] = brd_fit(b, shp, i, j);
        }
    }
    if (cnt == 0) {
        return false;
    }
    size_t idx = rand() % cnt;
    for (i = cnt = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            if (fit[i][j]) {
                if (cnt == idx) {
                    brd_put(b, shp, i, j);
                    return true;
                }
                cnt++;
            }
        }
    }
    return false;
}

bool fill_greedy(board_t *b, shape_t *shp) {
    size_t i, j, mscore = 0, mi, mj;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            if (brd_fit(b, shp, i, j)) {
                brd_put(b, shp, i, j);
                size_t score = brd_try_elim(b);
                brd_unput(b, shp, i, j);
                if (score > mscore) {
                    mscore = score;
                    mi = i, mj = j;
                }
            }
        }
    }
    if (mscore == 0) {
        return false;
    }
    brd_put(b, shp, mi, mj);
    return true;
}

#endif

bool max_capbility(board_t *const b, const shape_t *const shp) {
    size_t i, j, mscore = 0, mcap = 0, mi = 0, mj = 0;
    for (j = 0; j != COLS_BRD; j++) {
        for (i = 0; i != ROWS_BRD; i++) {
            if (brd_fit(b, shp, i, j)) {
                board_t *t = brd_dup(b);
                brd_put(t, shp, i, j);

                size_t score, cap = 0;
                score = brd_try_elim(t);
                brd_elim(t);
                size_t k;
                for (k = 1; k != g_shp_n; k++) { // skip consider plain 1x1 shape
                    shape_t *t_shp = g_shp_lst[k];
                    cap += fit_cnt(t, t_shp);
                }

                brd_close(t);
                if (cap > mcap) {
                    mcap = cap, mscore = score, mi = i, mj = j;
                } else if (cap == mcap && score > mscore) {
                    mscore = score, mi = i, mj = j;
                }
            }
        }
    }
    if (mscore == 0 && mcap == 0) {
        return false;
    }
    brd_put(b, shp, mi, mj);
    return true;
}

bool max_cap_score(board_t *b, shape_t *shp) {
    size_t i, j, mcap = 0, mi = -1, mj = -1;
    for (i = 0; i != ROWS_BRD; i++) {
        for (j = 0; j != COLS_BRD; j++) {
            if (brd_fit(b, shp, i, j)) {
                board_t *t = brd_dup(b);
                brd_put(t, shp, i, j);

                size_t cap;
                size_t k;
                for (k = cap = 0; k != g_shp_n; k++) {
                    board_t *t1 = brd_dup(t);
                    shape_t *t_shp = g_shp_lst[k];
                    cap += max_capbility(t1, t_shp);
                    brd_close(t1);
                }

                brd_close(t);
                if (cap > mcap) {
                    mcap = cap, mi = i, mj = j;
                }
            }
        }
    }
    if (mi == -1 && mi == -1) {
        return false;
    }
    brd_put(b, shp, mi, mj);
    return true;
}

bool brd_fill(board_t *b, shape_t *shp) {
    // return first_fit(b, shp);
    // return corner(b, shp);
    // return center(b, shp);
    // return fill_random(b, shp);
    // return fill_greedy(b, shp);
    return max_capbility(b, shp); // 780.291
    // return max_cap_score(b, shp);
}

/************** SHAPE *********************************************************/
shape_t *shp_create(size_t rows, size_t cols, const char *s) {
    shape_t *shp = malloc(sizeof(shape_t));
    shp->rows = rows;
    shp->cols = cols;
    shp->nblk = 0;
    shp->enc = 0;
    shp->b = malloc(sizeof(bool) * rows * cols);
    assert(rows * cols == strlen(s));
    size_t i;
    for (i = 0; i != rows * cols; i++) {
        shp->b[i] = s[i] == 'O';
        shp->nblk += shp->b[i];
        // shp->enc = (shp->enc << 1) + (!shp->b[i]); // for RK
        shp->enc = (shp->enc << 1) + (shp->b[i]); // for RK
    }
#ifdef ENC1D
    size_t j, k;
    shp->enc1d = malloc(sizeof(pm_enc_t) * rows);
    for (i = k = 0; i != rows; i++) {
        shp->enc1d[i] = 0;
        for (j = 0; j != cols; j++, k++) {
            shp->enc1d[i] = (shp->enc1d[i] << 1) + shp->b[k];
        }
    }
#endif
    return shp;
}

shape_t **shp_open(size_t *shp_n) {
    size_t i = 0;
    size_t n = 1 + // 1x1
               2 * 4 + // 1x2,2x1, 1x3, 3x1, 1x4, 4x1, 1x5, 5x1
               4 + 2 + // 2x2
               12 + // 2x3, 3x2
               2; // square 2x2, 3x3
    shape_t **shp_lst = malloc(sizeof(shape_t *) * n);
    shp_lst[i++] = shp_create(1, 1, "O"); // O
    // OO O
    //    O
    shp_lst[i++] = shp_create(1, 2, "OO");
    shp_lst[i++] = shp_create(2, 1, "OO");
    //  O  O    OO  OO C(4, 1)=4
    // OO  OO    O  O
    shp_lst[i++] = shp_create(2, 2, " OOO");
    shp_lst[i++] = shp_create(2, 2, "O OO");
    shp_lst[i++] = shp_create(2, 2, "OO O");
    shp_lst[i++] = shp_create(2, 2, "OOO ");
    // O    O
    //  O  O
    shp_lst[i++] = shp_create(2, 2, " OO ");
    shp_lst[i++] = shp_create(2, 2, "O  O");

    // C(2*3, 4) = 6*5/2*1=15?
    shp_lst[i++] = shp_create(2, 3,
                              " OO"
                              "OO ");
    shp_lst[i++] = shp_create(2, 3,
                              "OO "
                              " OO");
    shp_lst[i++] = shp_create(3, 2,
                              " O"
                              "OO"
                              "O ");
    shp_lst[i++] = shp_create(3, 2,
                              "O "
                              "OO"
                              " O");
    // O
    //OOO 4
    shp_lst[i++] = shp_create(2, 3,
                              " O "
                              "OOO");
    shp_lst[i++] = shp_create(2, 3,
                              "OOO"
                              " O ");
    shp_lst[i++] = shp_create(3, 2,
                              " O"
                              "OO"
                              " O");
    shp_lst[i++] = shp_create(3, 2,
                              "O "
                              "OO"
                              "O ");
    //  O 4
    //OOO
    shp_lst[i++] = shp_create(2, 3,
                              "  O"
                              "OOO");
    shp_lst[i++] = shp_create(2, 3,
                              "OOO"
                              "  O");
    shp_lst[i++] = shp_create(3, 2,
                              " O"
                              " O"
                              "OO");
    shp_lst[i++] = shp_create(3, 2,
                              "O "
                              "O "
                              "OO");

    shp_lst[i++] = shp_create(1, 3, "OOO");
    shp_lst[i++] = shp_create(3, 1, "OOO");
    shp_lst[i++] = shp_create(1, 4, "OOOO");
    shp_lst[i++] = shp_create(4, 1, "OOOO");
    shp_lst[i++] = shp_create(1, 5, "OOOOO");
    shp_lst[i++] = shp_create(5, 1, "OOOOO");

    shp_lst[i++] = shp_create(2, 2, "OOOO");
    shp_lst[i++] = shp_create(3, 3, "OOOOOOOOO");

    *shp_n = n;
    return shp_lst;
}

void shp_close(shape_t **shp_lst, size_t shp_n) {
    size_t i;
    for (i = 0; i != shp_n; i++) {
#ifdef ENC1D
        free(shp_lst[i]->enc1d);
#endif
        free(shp_lst[i]->b);
        free(shp_lst[i]);
    }
    free(shp_lst);
}

void shp_show(const shape_t *const s, FILE *fp) {
    size_t i, j;
    fprintf(fp, "%zux%zu enc=%lx\n", s->rows, s->cols, s->enc);
#ifdef ENC1D
    fprintf(fp, "enc1d=");
    for (i = 0; i != s->rows; i++) {
        fprintf(fp, " %lx", s->enc1d[i]);
    }
    fprintf(fp, "\n");
#endif
    for (i = 0; i != s->rows; i++) {
        for (j = 0; j != s->cols; j++) {
            fprintf(fp, "%c ", s->b[i * s->cols + j] ? 'O' : ' ');
        }
        fprintf(fp, "\n");
    }
}

#if 0
int test() {
    board_t *b = brd_open();
    brd_show(b);

    size_t shp_n, i;
    shape_t **shp_lst = shp_open(&shp_n);
    for (i = 0; i != shp_n; i++) {
        shp_show(shp_lst[i]);
    }

    shape_t *one = shp_lst[0];
    shape_t *s0 = shp_lst[1];
    shape_t *sqr2 = shp_lst[shp_n - 2];

    bool t = brd_fit(b, one, 2, 2);
    printf("t=%s\n", t ? "true" : "false");

    brd_put(b, one, 2, 2);
    brd_show(b);

    t = brd_fit(b, s0, 1, 0);
    printf("t=%s\n", t ? "true" : "false");
    brd_put(b, s0, 1, 0);
    brd_show(b);

    brd_put(b, sqr2, 4, 0);
    brd_put(b, sqr2, 4, 2);
    brd_put(b, sqr2, 4, 4);
    brd_put(b, sqr2, 0, 4);
    brd_put(b, sqr2, 2, 4);
    brd_put(b, sqr2, 0, 4);
    brd_put(b, sqr2, 6, 4);
    brd_put(b, sqr2, 4, 6);
    brd_show(b);

    brd_elim(b);
    brd_show(b);

    shp_close(shp_lst, shp_n);
    brd_close(b);
    return 0;
}

#endif

FILE *fp_update(size_t frame, FILE *fp) {
    if (fp) {
        fflush(fp);
        fclose(fp);
    }
    char fn[10];
    sprintf(fn, "%04zu.txt", frame);
    fp = fopen(fn, "w");
    return fp;
}

int test_fill_fn_seed(unsigned int seed, bool show) {
    g_shp_lst = shp_open(&g_shp_n);

    board_t *b = brd_open();
    unsigned int shp_seed = seed;

    FILE *fp = NULL;
    size_t round, frame;
    for (round = frame = 0;; round++) {
        size_t shp_rnd = rand_r(&shp_seed) % g_shp_n;
        shape_t *shp = g_shp_lst[shp_rnd];
        if (show) {
            fp = fp_update(frame++, fp);
            brd_show(b, fp);
            shp_show(shp, fp);
        }

        bool fill = brd_fill(b, shp);
        if (!fill) {
            break;
        }
        if (show) {
            fp = fp_update(frame++, fp);
            brd_show(b, fp);
            shp_show(shp, fp);
        }

        brd_elim(b);
        if (show) {
            fp = fp_update(frame++, fp);
            brd_show(b, fp);
        }
    }

    size_t score = b->score;
    if (fp) {
        fclose(fp);
    }
    brd_close(b);
    shp_close(g_shp_lst, g_shp_n);
    return score;
}

int test_bench() {
    unsigned int seed;
    size_t score = 0, round = 1000;
    for (seed = 0; seed != round; seed++) {
        size_t this;
        score += this = test_fill_fn_seed(seed, 0);
        printf("seed=%u score=%zu avg=%f\n", seed, this, score / (seed + 1.));
    }
    printf("avg score=%f\n", score / (round + 0.));
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1 || strcmp(argv[1], "sample") == 0) {
        test_fill_fn_seed(68, 1);
    } else if (strcmp(argv[1], "bench") == 0) {
        test_bench();
    } else {
        printf("unknown sub-test=%s\n", argv[1]);
    }
    return 0;
}
