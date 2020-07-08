/**
 * model - view
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#include <unistd.h>

#include <curses.h>

#ifdef DEBUG
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...)
#endif

#define PI (3.1415926)

static inline int min(int a, int b) {
    return a < b ? a : b;
}

/** basic rand function */
/** [0, 1) */
static inline double urand() {
    return rand() / (RAND_MAX + 0.);
}

/** [b, b + w) */
static inline double randb(double b, double w) {
    return b + urand() * w;
}

/** [m - w/2, m + w/2) */
static inline double randm(double m, double w) {
    return m + (urand() - 0.5) * w;
}

/** basic vector operation */
typedef struct {
    double x, y;
} Vec2;

/** descartes */
static inline Vec2 vec2_cons(double x, double y) {
    Vec2 p = {.x = x, .y = y};
    return p;
}

/** polar */
static inline Vec2 vec2_cons_polar(double l, double a) {
    Vec2 p = {.x = l * cos(a), .y = l * sin(a)};
    return p;
}

static inline Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 c = {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
    return c;
}

static inline Vec2 vec2_swp(Vec2 a) {
    Vec2 c = {.x = a.y, .y = a.x};
    return c;
}

static inline Vec2 vec2_sub(Vec2 a, Vec2 b) {
    Vec2 c = {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
    return c;
}

static inline Vec2 vec2_mul(Vec2 a, Vec2 b) {
    Vec2 c = {
        .x = a.x * b.x - a.y * b.y,
        .y = a.y * b.x + a.x * b.y,
    };
    return c;
}

static inline Vec2 vec2_zero() {
    Vec2 c = {.x = 0, .y = 0};
    return c;
}

static inline Vec2 vec2_neg(Vec2 a) {
    Vec2 c = {.x = -a.x, .y = -a.y};
    return c;
}

static inline double vec2_abs2(Vec2 a) {
    return a.x * a.x + a.y * a.y;
}

static inline double vec2_abs(Vec2 a) {
    // TODO
    return 0;
}

static inline double vec2_arg(Vec2 a) {
    // TODO
    return 0;
}

static inline Vec2 vec2_scale(Vec2 a, double k) {
    Vec2 c = {.x = a.x * k, .y = a.y * k};
    return c;
}

static inline Vec2 vec2_rescale(Vec2 a, double len) {
    double l = vec2_abs(a);
    return vec2_scale(a, len / l);
}

static inline Vec2 vec2_rescale2(Vec2 a, double len2) {
    double l2 = vec2_abs2(a);
    return vec2_scale(a, sqrt(len2 / l2));
}

static inline double vec2_slope(Vec2 a) {
    return a.y / a.x;
}

static inline double vec2_slope2(Vec2 a, Vec2 b) {
    Vec2 c = vec2_sub(a, b);
    return vec2_slope(c);
}

static inline Vec2 vec2_round(Vec2 a) {
    Vec2 c = {.x = round(a.x), .y = round(a.y)};
    return c;
}

/** helper draw function */
static inline void drawpt(Vec2 p, char *s) {
    p = vec2_round(p);
    mvprintw(p.y, p.x, s);
}

/** Bresenham draw line algo */
void drawline(Vec2 a, Vec2 b, char *s) {
    LOG("line %f,%f to %f,%f\n", a.x, a.y, b.x, b.y);
    bool steep = abs(b.y - a.y) > abs(b.x - a.x);
    if (steep) {
        a = vec2_swp(a);
        b = vec2_swp(b);
    }
    if (a.x > b.x) {
        Vec2 t = a;
        a = b;
        b = t;
    }
    // from A to B, and slope <= 1
    double dx = b.x - a.x;
    double dy = abs(b.y - a.y);
    double de = dy / dx;
    double e = 0;
    int step = a.y < b.y ? 1 : -1;
    int x, y;
    Vec2 pt;
    for (x = round(a.x), y = a.y; x <= round(b.x); x++) {
        pt.x = x, pt.y = y;
        if (steep) {
            pt = vec2_swp(pt);
        }
        drawpt(pt, s);
        e += de;
        if (e >= 0.5) {
            y += step;
            e -= 1.0;
        }
    }
}

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

static inline bool in1d(int x, int a, int b) {
    return a <= x && x < b;
}

static inline bool in(Mat *p, int x, int y) {
    return in1d(x, 0, p->w) && in1d(y, 0, p->h);
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
            int cond = (urand() <= 0.04);
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
            // generate life with low probablity
            if (!idx(m, i, j)) {
                int cond = (urand() <= 0.001);
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
    /** TODO: opt to avoid putrow */
    RuleX *p = g;
    int x, y;
    for (y = 0; y != p->h - 1; y++) {
        putrow(p->m, y, y + 1);
    }
    y = p->h - 2;
    for (x = 0; x != p->w; x++) {
        int a = idxEx(p->m, x - 1, y) * 4 + idxEx(p->m, x, y) * 2 + idxEx(p->m, x + 1, y);
        put(p->m, x, p->h - 1, in1d(a, 1, 5));
        /** 30 = [0, 1, 1, 1,  1, 0, 0, 0], when a is [1..5), map to 1, else 0 */
    }
    return 0;
}

/** body3 ********************************************************************/

#define BODY_NUM 3
#define TRACE_NUM 100
typedef struct {
    int times;
    int head; // head in trace
    Vec2 pt[BODY_NUM][TRACE_NUM];
    Vec2 v[BODY_NUM];
    int w, h;
} Body3;

void *body3_create(int w, int h) {
    LOG("win=%d,%d\n", w, h);
    Body3 *p = malloc(sizeof(Body3));
    p->w = w, p->h = h;
    p->times = p->head = 0;
    double w_ = w, h_ = h;
    int i, hd = p->head;
    p->pt[0][hd] = vec2_cons(w_ / 2 * BODY_NUM, h_ * BODY_NUM / 2);
    for (i = 1; i != BODY_NUM; i++) {
        // keep mass central is in middle
        p->pt[i][hd] = vec2_cons(randm(w_ / 2, w_ / 2), randm(h_ / 2, h_ / 2));
        p->pt[0][hd] = vec2_sub(p->pt[0][hd], p->pt[i][hd]);
    }
    p->v[0] = vec2_cons(0, 0);
    for (i = 1; i != BODY_NUM; i++) {
        p->v[i] = vec2_cons(randm(0, w_ * 0.005), randm(0, h_ * 0.005));
        p->v[0] = vec2_sub(p->v[0], p->v[i]);
    }
    return p;
}

void body3_destroy(void *g) {
    free(g);
}

int body3_iter(void *g) {
    Body3 *p = g;
    int old = p->head;
    int new = (p->head + 1) % (TRACE_NUM);
    // new = old + delta
    // force
    Vec2 f[BODY_NUM][BODY_NUM];
    memset(f, 0x00, sizeof(f));
    int i, j;
    for (i = 0; i != BODY_NUM - 1; i++) {
        for (j = i + 1; j != BODY_NUM; j++) {
            Vec2 fij = vec2_sub(p->pt[i][old], p->pt[j][old]); // from i to j
            double dist2 = vec2_abs2(fij);
            f[i][j] = vec2_rescale2(fij, 1. / dist2);
            f[j][i] = vec2_neg(f[i][j]);
        }
    }
    for (i = 0; i != BODY_NUM; i++) {
        // velocity
        Vec2 s;
        for (j = 0, s = vec2_zero(); j != BODY_NUM; j++) {
            s = vec2_add(s, f[j][i]);
        }
        s = vec2_scale(s, 0.5);
        p->v[i] = vec2_add(p->v[i], s);
        // location
        p->pt[i][new] = vec2_add(p->pt[i][old], p->v[i]);
        LOG("i=%d f=%f,%f v=%f,%f pt=%f,%f\n",
            i, s.x, s.y, p->v[i].x, p->v[i].y, p->pt[i][new].x, p->pt[i][new].y);
        p->v[i] = vec2_add(p->v[i], s);
    }
    p->head = new;
    p->times++;
    return 0;
}

int body3_draw(void *g) {
    char s[100];
    Body3 *p = g;
    int hd = p->head;
    int i, j;
    for (i = 0; i != BODY_NUM; i++) {
        int x = round(p->pt[i][hd].x), y = round(p->pt[i][hd].y);
        LOG("%d,%d\n", x, y);
        sprintf(s, "%d", i);
        for (j = 0; j != min(p->times, TRACE_NUM - 1); j++) {
            int k = (hd + TRACE_NUM - j) % TRACE_NUM;
            int k1 = (hd + TRACE_NUM - j - 1) % TRACE_NUM;
            drawline(p->pt[i][k], p->pt[i][k1], ".");
        }
        drawpt(p->pt[i][hd], s);
    }
    return 0;
}

/** square */
#define VERTEX_NUM 5
typedef struct {
    int w, h;
    Vec2 vertex[VERTEX_NUM];
    Vec2 origin;
    Vec2 rot;
} Sqr;

void *sqr_create(int w, int h) {
    Sqr *p = malloc(sizeof(Sqr));
    p->w = w, p->h = h;
    p->origin = vec2_cons(w / 2., h / 2.);
    double len = h / 3.;
    Vec2 rotn = vec2_cons_polar(1, (2 * PI) / VERTEX_NUM);
    p->vertex[0] = vec2_cons(len, 0);
    int i;
    for (i = 1; i != VERTEX_NUM; i++) {
        p->vertex[i] = vec2_mul(rotn, p->vertex[i - 1]);

    }
    p->rot = vec2_cons_polar(1, 0.01);
    return p;
}

void sqr_destory(void *g) {
    free(g);
}

int sqr_iter(void *g) {
    Sqr *p = g;
    int i;
    for (i = 0; i != VERTEX_NUM; i++) {
        p->vertex[i] = vec2_mul(p->rot, p->vertex[i]);
    }
    return 0;
}

int sqr_draw(void *g) {
    Sqr *p = g;
    int i, j;
#if 1 // draw polygon
    for (i = 0; i != VERTEX_NUM; i++) {
        Vec2 pt0 = vec2_add(p->origin, p->vertex[i]);
        Vec2 pt1 = vec2_add(p->origin, p->vertex[(i + 1) % VERTEX_NUM]);
        drawline(pt0, pt1, ".");
    }
#else // draw complete graph
    for (i = 0; i != VERTEX_NUM; i++) {
        for (j = i + 1; j != VERTEX_NUM; j++) {
            Vec2 pt0 = vec2_add(p->origin, p->vertex[i]);
            Vec2 pt1 = vec2_add(p->origin, p->vertex[j]);
            drawline(pt0, pt1, ".");
        }
    }
#endif
    for (i = 0; i != VERTEX_NUM; i++) {
        Vec2 pt =  vec2_add(p->origin, p->vertex[i]);
        drawpt(pt, "o");
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
    {"square", sqr_create, sqr_iter, sqr_draw, sqr_destory, 30},
    {"body3", body3_create, body3_iter, body3_draw, body3_destroy, 30},
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
