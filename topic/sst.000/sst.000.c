/**
 * model - view
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include <unistd.h>

#include <curses.h>

/** basic two-dim array ******************************************************/
typedef char T;
typedef struct {
    int w, h, wh;
    T a[0];
} Mat;

// destory: free(Mat *)
Mat *m_create(int w, int h) {
    assert(w > 0);
    assert(h > 0);
    int wh = w * h;
    Mat *p = malloc(sizeof(Mat) + sizeof(T) * wh);
    p->w = w, p->h = h;
    memset(p->a, 0x00, sizeof(T) * wh);
    return p;
}

static inline T idx(Mat *p, int x, int y) {
    return p->a[y * p->w + x];
}

static inline T idx1d(Mat *p, int i) {
    return p->a[i];
}

static inline bool in(Mat *p, int x, int y) {
    return (0 <= x) && (x < p->w) && (0 <= y) && (y < p->h);
}

static inline bool eq(Mat *p, T v, int x, int y) {
    return in(p, x, y) && (v == idx(p, x, y));
}

static inline void put(Mat *p, int x, int y, T v) {
    p->a[y * p->w + x] = v;
}

static inline void putrow(Mat *p, int y0, int y1) {
    memcpy(&p->a[y0 * p->w], &p->a[y1 * p->w], sizeof(T) * p->w);
}

static inline T idxEx(Mat *p, int x, int y) {
    return in(p, x, y) ? idx(p, x, y) : 0;
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

static inline uint32_t m_adj8sum(Mat *p, int x, int y) {
    return idxEx(p, x - 1, y - 1) + idxEx(p, x - 1, y) + idxEx(p, x - 1, y + 1) +
           idxEx(p, x, y - 1) + idxEx(p, x, y + 1) +
           idxEx(p, x + 1, y - 1) + idxEx(p, x + 1, y) + idxEx(p, x + 1, y + 1);
}

static inline int true_draw(Mat *m, char c) {
    char trues[2] = {c, '\0'};
    int i, j;
    for (j = 0; j != m->h; j++) {
        for (i = 0; i != m->w; i++) {
            if (idx(m, i, j)) {
                mvprintw(j, i, trues);
            }
        }
    }
    return 0;
}

/** breakout game ************************************************************/
typedef struct {
    Mat *m;
    int w, h;
    int x, y;
    int cx, cy;
} Breakout;

enum {
    BREAKOUT_BG = 0,
    BREAKOUT_BLK = 1,
};

void breakout_destory(void *g) {
    Breakout *p = g;
    free(p->m);
    free(p);

}

void *breakout_create(int w, int h) {
    Breakout *p = malloc(sizeof(Breakout));
    p->m = m_create(w, h);
    p->w = w;
    p->h = h;
    p->x = p->y = 0;
    p->cx = p->cy = 1;
    int block = 10;
    int i, j;
    for (j = block; j != p->h - block; j++) {
        for (i = block; i != p->w - block; i++) {
            put(p->m, i, j, BREAKOUT_BLK);
        }
    }
    return p;
}

int breakout_iter(void *g) {
    Breakout *p = g;
    p->x += p->cx;
    p->y += p->cy;
    if (p->x == 0 || p->x == p->w) {
        p->cx = -p->cx;
    }
    if (p->y == 0 || p->y == p->h) {
        p->cy = -p->cy;
    }
    if (idx(p->m, p->x, p->y)) {
        put(p->m, p->x, p->y, BREAKOUT_BG);
        if (idx(p->m, p->x - 1, p->y) || idx(p->m, p->x + 1, p->y)) {
            put(p->m, p->x - 1, p->y, BREAKOUT_BG);
            put(p->m, p->x + 1, p->y, BREAKOUT_BG);
            p->cx = -p->cx;
        }
        if (idx(p->m, p->x, p->y - 1) || idx(p->m, p->x, p->y + 1)) {
            put(p->m, p->x, p->y - 1, BREAKOUT_BG);
            put(p->m, p->y, p->y + 1, BREAKOUT_BG);
            p->cy = -p->cy;
        }
    }
    if (m_sum(p->m) == 0) {
        return 1;
    }
    return 0;
}

int breakout_draw(void *g) {
    Breakout *p = g;
    mvprintw(p->y, p->x, "O");
    int i, j;
    for (j = 0; j != p->h; j++) {
        for (i = 0; i != p->w; i++) {
            if (idx(p->m, i, j) == BREAKOUT_BLK) {
                mvprintw(j, i, "X");
            }
        }
    }
    return 0;
}

/** life game ****************************************************************/

enum {
    LIFE_NONE = 0,
    LIFE_ONE = 1,
};

void *life_create(int w, int h) {
    Mat *m = m_create(w, h);
    int i, j;
    for (j = 0; j != m->h; j++) {
        for (i = 0; i != m->w; i++) {
            uint64_t r = rand();
            int cond = (r * 100 <= 4llu * RAND_MAX);
            put(m, i, j, cond ? LIFE_ONE : LIFE_NONE);
        }
    }
    return m;
}

void life_destroy(void *g) {
    free(g);
}

// TODO: opt adj8sum for entire grid
int life_iter(void *g) {
    Mat *m = g;
    int i, j;
    for (j = 0; j != m->h; j++) {
        for (i = 0; i != m->w; i++) {
            int x = m_adj8sum(m, i, j);
            switch (x) {
                case 0:
                case 1: put(m, i, j, LIFE_NONE); break; // die lonely
                case 2: break; // do nothing
                case 3: put(m, i, j, LIFE_ONE); break; // do nothing, or born
                case 4:
                case 5:
                case 6:
                case 7:
                case 8: put(m, i, j, LIFE_NONE); break; // die as crowd
            }
            if (!idx(m, i, j)) {
                uint64_t r = rand();
                int cond = (r * 1000 <= 1 * RAND_MAX);
                if (cond) {
                    put(m, i, j, LIFE_ONE);
                }
            }
        }
    }
    return 0;
}

int life_draw(void *g) {
    Mat *m = g;
    true_draw(m, 'O');
    return 0;
}

/** rule30 *******************************************************************/

typedef struct {
    Mat *m;
    int times, w, h;
} RuleX;

void rulex_destroy(void *g) {
    RuleX *p = g;
    free(p->m);
    free(p);
}

void *rulex_create(int w, int h) {
    RuleX *p = malloc(sizeof(RuleX));
    p->m = m_create(w, h);
    p->w = w, p->h = h, p->times = 0;
    put(p->m, w / 2, p->h - 1, true);
    return p;
}

int rulex_draw(void *g) {
    RuleX *p = g;
    return true_draw(p->m, '*');
}

int rulex_iter(void *g) {
    RuleX *p = g;
    int x, y;
    for (y = 0; y != p->h - 1; y++) {
        putrow(p->m, y, y + 1);
    }
    y = p->h - 2;
    for (x = 0; x != p->w; x++) {
        int a = idxEx(p->m, x - 1, y) * 4 + idxEx(p->m, x, y) * 2 + idxEx(p->m, x + 1, y);
        put(p->m, x, p->h - 1, 1 <= a && a <= 4);
        /** 30 = [0, 1, 1, 1,  1, 0, 0, 0], when a is [1..4], map to 1, else 0 */
    }
    return 0;
}

/** General game framework & canvas part *************************************/
/**
 * game_create
 * game_iter
 * game_draw
 * game_destory
 */
typedef void *(*game_create_fn)(int w, int h);
typedef int (*game_iter_fn)(void *p);
typedef int (*game_draw_fn)(void *p);
typedef void (*game_destory_fn)(void *p);
typedef struct {
    char *name;
    game_create_fn create;
    game_iter_fn iter;
    game_draw_fn draw;
    game_destory_fn destory;
    int freq;
} Game;

Game gamelst[] = {
    {"rulex", rulex_create, rulex_iter, rulex_draw, rulex_destroy, 1},
    {"life", life_create, life_iter, life_draw, life_destroy, 1},
    {"breakout", breakout_create, breakout_iter, breakout_draw, breakout_destory, 30},
    {NULL},
};

Game *select_game(char *name) {
    int i;
    for (i = 0; gamelst[i].name != NULL; i++) {
        if (strcasecmp(name, gamelst[i].name) == 0) {
            return gamelst + i;
        }
    }
    return NULL;
}

void canvas_init(int *pw, int *ph) {
    assert(pw != NULL);
    assert(ph != NULL);
    initscr();
    noecho();
    curs_set(FALSE);
    int y, x;
    // !!! This is macro, instead of function-calling
    getmaxyx(stdscr, y, x); // stdscr from initscr, get max_y, max_x
    *pw = x;
    *ph = y;
}

void canvas_deinit() {
    endwin();
}

void canvas_preupdate() {
    clear();
}

static int g_interval;
void canvas_postupdate() {
    refresh();
    usleep(g_interval);
}

int main(int argc, char **argv) {
    int w, h;
    canvas_init(&w, &h);
    char *name = argc == 1 ? "breakout" : argv[1];
    Game *game = select_game(name);
    void *g = game->create(w, h);
    g_interval = 1000000 / game->freq;

    while (1) {
        canvas_preupdate();
        game->iter(g);
        game->draw(g);
        canvas_postupdate();
    }

    game->destory(g);
    canvas_deinit();
    return 0;
}
