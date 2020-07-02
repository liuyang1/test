/** mine sweeper
 * 0, board
 * 1, + visible + click
 * 2, + terminal in console
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct {
    int32_t p, q;
} R;

int cmp(R a, R b) {
    return (int32_t)((int64_t)(a.p) * b.q - (int64_t)(b.p) * (a.q));
}

typedef char Elem;
typedef struct {
    int w, h, wh;
    Elem a[0];
} Mat;

void *m_create(int w, int h) {
    int wh = w * h;
    Mat *p = malloc(sizeof(Mat) + sizeof(Elem) * wh);
    p->w = w, p->h = h;
    memset(p->a, 0x00, sizeof(Elem) * wh);
    return p;
}

static inline Elem idx(Mat *p, int x, int y) {
    return p->a[y * p->w + x];
}

static inline Elem idx1d(Mat *p, int i) {
    return p->a[i];
}

static inline bool inRegion(Mat *p, int x, int y) {
    return (0 <= x) && (x < p->w) && (0 <= y) && (y < p->h);
}

static inline bool eqX(Mat *p, Elem v, int x, int y) {
    return inRegion(p, x, y) && (v == idx(p, x, y));
}

static inline void put(Mat *p, int x, int y, Elem v) {
    p->a[y * p->w + x] = v;
}

void m_show(Mat *p) {
    if (p == NULL) {
        printf("mat=nil\n");
        return;
    }
    printf("mat=%p %d*%d\n", p, p->w, p->h);
    int i, j;
    for (j = 0; j != p->h; j++) {
        for (i = 0; i != p->w; i++) {
            printf("%d ", idx(p, i, j));
        }
        printf("\n");
    }
}

static inline uint32_t m_sum(Mat *p) {
    int i, wh = p->h * p->w;
    uint32_t s;
    for (i = s = 0; i != wh; i++) {
        s += idx1d(p, i);
    }
    return s;
}

typedef struct {
    int w, h;
    int mines;
    Mat *mine;
    Mat *stat;
    Mat *vis;
    Mat *mark;
} Board;


void *brd_create(int w, int h, int mines) {
    int wh = w * h;
    Board *p = malloc(sizeof(Board));
    p->w = w, p->h = h, p->mines = mines;
    p->mine = m_create(w, h);
    p->stat = m_create(w, h);
    p->vis = m_create(w, h);
    p->mark = m_create(w, h);
    int i, j;
    uint64_t a = mines, b = wh;
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            uint64_t r = rand();
            int cond = (r * b <= a * RAND_MAX);
            // printf("%llu/%llu %llu/%llu cond=%d\n", r, RAND_MAX, a, b, cond);
            put(p->mine, i, j, cond);
            a -= cond, b--;
        }
    }
    // return p;
    if (a != 0) {
        printf("res mines=%" PRIu64 "\n", a);
    }
    for (j = 0; j != h; j++) {
        for (i = 0; i != w; i++) {
            if (idx(p->mine, i, j)) {
                continue;
            }
            int x0 = eqX(p->mine, 1, i - 1, j - 1);
            int x1 = eqX(p->mine, 1, i, j - 1);
            int x2 = eqX(p->mine, 1, i + 1, j - 1);
            int x3 = eqX(p->mine, 1, i - 1, j);
            int x4 = 0;
            int x5 = eqX(p->mine, 1, i + 1, j);
            int x6 = eqX(p->mine, 1, i - 1, j + 1);
            int x7 = eqX(p->mine, 1, i, j + 1);
            int x8 = eqX(p->mine, 1, i + 1, j + 1);
            int s = x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;
            put(p->stat, i, j, s);
            // printf("%d,%d %d %d,%d,%d %d,%d,%d %d,%d,%d\n", i, j, IDX(i, j),
            //        x0, x1, x2, x3, x4, x5, x6, x7, x8);
        }
    }
    return p;
}

void brd_destroy(Board *p) {
    free(p->mine);
    free(p->stat);
    free(p->vis);
    free(p->mark);
    free(p);
}

void brd_show(Board *p) {
    printf("board p=%p mine=%d\n", p, p->mines);
    m_show(p->mine);
    m_show(p->stat);
    m_show(p->vis);
    m_show(p->mark);
}

int brd_sniff(Board *p, int x, int y) {
    if (!inRegion(p->mine, x, y)) {
        return 0;
    }
    if (idx(p->mine, x, y)) {
        return 0;
    }
    put(p->vis, x, y, 1);
    if (idx(p->stat, x, y) != 0) {
        return 0;
    }
#define TRY_SNIFF(p, x, y) { if (!idx(p->vis, x, y)) { brd_sniff(p, x, y); } }
    TRY_SNIFF(p, x - 1, y);
    TRY_SNIFF(p, x, y - 1);
    TRY_SNIFF(p, x + 1, y);
    TRY_SNIFF(p, x, y + 1);
    return 0;
}

int brd_invis(Board *p) {
    return p->w * p->h - m_sum(p->vis);
}

enum {
    rBOOM = 0,
    rKO = 1,
    rUND = -1,
};
int brd_click(Board *p, int x, int y) {
    put(p->vis, x, y, 1);
    if (idx(p->mine, x, y)) {
        printf("BOOM! at %d,%d\n", x, y);
        return rBOOM;
    } else {
        brd_sniff(p, x, y);
        int s = brd_invis(p);
        if (s == p->mines) {
            printf("KO!\n");
            return rKO;
        }
        return rUND;
    }
}

void brd_probe(Board *p) {
    int i, j;
    for (j = 0; j != p->h; j++ ) {
        for (i = 0; i != p->w; i++) {
            if (idx(p->mine, i, j)) {
                printf("X ");
            } else {
                printf("%d ", idx(p->stat, i, j));
            }

        }
        printf("\n");
    }
}

void brd_mark(Board *p, int x, int y) {
    put(p->mark, x, y, 1);
}

void brd_display(Board *p) {
    int i, j;
    printf("P/mine/invis=%d/%d/%d\n", m_sum(p->mark), p->mines, brd_invis(p));
    // show index
    printf("%c: ", ' ');
    for (j = 0; j != p->w; j++) {
        printf("%d ", j);
        if ((j + 1) % 5 == 0) {
            printf(" ");
        }
    }
    printf("\n");
    for (j = 0; j != p->h; j++) {
        printf("%d: ", j);
        for (i = 0; i != p->w; i++) {
            char c = '\0';
            if (idx(p->mark, i, j)) {
                c = 'P';
            } else if (!idx(p->vis, i, j)) {
                c = '*';
            } else if (idx(p->mine, i, j)) {
                c = 'X';
            } else {
                c = '0' + idx(p->stat, i, j);
            }
            printf("%c ", c);
            if ((i + 1) % 5 == 0) {
                printf(" ");
            }
        }
        if ((j + 1) % 5 == 0) {
            printf("\n");
        }
        printf("\n");
    }
}

int sample() {
    void *p = brd_create(8, 6, 10);
    // brd_show(p);
    brd_probe(p);
    // brd_display(p);
    brd_click(p, 5, 5);
    brd_click(p, 0, 5);
    brd_click(p, 5, 0);
    brd_display(p);
    brd_destroy(p);
    return 0;
}

int play_term() {
    FILE *hst = fopen("history", "w");
    // void *p = brd_create(8, 6, 10);
    void *p = brd_create(3, 3, 1);
    int x, y;
    int ret;
    for (ret = rUND; ret == rUND;) {
        brd_display(p);
        char act[10];
        printf("act=[c,p] x y : ");
        int r = scanf("%s%d%d", act, &x, &y);
        if (r != 3) {
            printf("user stop: r=%d act=%s\n", r, act);
            break;
        }
        fprintf(hst, "%s %d %d\n", act, x, y);
        switch (act[0]) {
            case 'C':
            case 'c':
                ret = brd_click(p, x, y); break;
            case 'p':
            case 'P':
                brd_mark(p, x, y); break;
            default:
                printf("unknown act=%s\n", act);
        }
    }
    brd_display(p);
    brd_probe(p);
    brd_destroy(p);
    fclose(hst);
    return ret;
}

int main() {
    play_term();
    return 0;
}
