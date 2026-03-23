/**
 * ScreenSaver-Terminal (sst)
 *
 * All games draw onto a logical framebuffer at 2x vertical resolution.
 * Terminal chars are ~2:1 (height:width), so W*H screen becomes W*(H*2)
 * logical canvas with approximately square pixels.
 * Downsampled via half-block chars:
 *   fancy:  ▀ ▄ █   (Unicode)
 *   compat: ^ _ #    (ASCII)
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <curses.h>

#define PI (3.1415926)

static inline int min_i(int a, int b) { return a < b ? a : b; }
static inline int max_i(int a, int b) { return a > b ? a : b; }
static inline int clamp_i(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }
static inline double urand() { return rand() / (RAND_MAX + 0.); }
static inline double randb(double b, double w) { return b + urand() * w; }
static inline double randm(double m, double w) { return m + (urand() - 0.5) * w; }

enum { RENDER_COMPAT = 0, RENDER_FANCY = 1 };
static int g_render_mode = RENDER_COMPAT;

/** vector *******************************************************************/
typedef struct { double x, y; } Vec2;
static inline Vec2 v2(double x, double y) { return (Vec2){x, y}; }
static inline Vec2 v2polar(double l, double a) { return v2(l*cos(a), l*sin(a)); }
static inline Vec2 v2add(Vec2 a, Vec2 b) { return v2(a.x+b.x, a.y+b.y); }
static inline Vec2 v2sub(Vec2 a, Vec2 b) { return v2(a.x-b.x, a.y-b.y); }
static inline Vec2 v2neg(Vec2 a) { return v2(-a.x, -a.y); }
static inline Vec2 v2scale(Vec2 a, double k) { return v2(a.x*k, a.y*k); }
static inline Vec2 v2zero() { return v2(0, 0); }
static inline double v2abs2(Vec2 a) { return a.x*a.x + a.y*a.y; }
static inline double v2abs(Vec2 a) { return sqrt(v2abs2(a)); }
static inline Vec2 v2mul(Vec2 a, Vec2 b) {
    return v2(a.x*b.x - a.y*b.y, a.y*b.x + a.x*b.y);
}

/** framebuffer **************************************************************/
typedef struct { int w, h; char *px; } FB;
static FB g_fb;

static void fb_init(int w, int h) {
    g_fb.w = w; g_fb.h = h;
    g_fb.px = calloc(w * h, 1);
}
static void fb_free() { free(g_fb.px); }
static void fb_clear() { memset(g_fb.px, 0, g_fb.w * g_fb.h); }
static inline bool fb_in(int x, int y) { return x>=0 && x<g_fb.w && y>=0 && y<g_fb.h; }
static inline char fb_get(int x, int y) { return fb_in(x,y) ? g_fb.px[y*g_fb.w+x] : 0; }
static inline void fb_set(int x, int y, char v) { if (fb_in(x,y)) g_fb.px[y*g_fb.w+x]=v; }

static void fb_pt(Vec2 p) { fb_set((int)round(p.x), (int)round(p.y), 1); }

static void fb_line(Vec2 a, Vec2 b) {
    int x0=(int)round(a.x), y0=(int)round(a.y);
    int x1=(int)round(b.x), y1=(int)round(b.y);
    int dx=abs(x1-x0), dy=abs(y1-y0);
    int sx=x0<x1?1:-1, sy=y0<y1?1:-1, err=dx-dy;
    for (;;) {
        fb_set(x0, y0, 1);
        if (x0==x1 && y0==y1) break;
        int e2=2*err;
        if (e2 > -dy) { err-=dy; x0+=sx; }
        if (e2 <  dx) { err+=dx; y0+=sy; }
    }
}

static void fb_blit(int scr_w, int scr_h) {
    int x, y;
    for (y = 0; y < scr_h; y++) {
        int ly = y * 2;
        for (x = 0; x < scr_w; x++) {
            char top = fb_get(x, ly);
            char bot = fb_get(x, ly+1);
            if (!top && !bot) continue;
            if (g_render_mode == RENDER_FANCY) {
                if (top && bot)       mvprintw(y, x, "\xe2\x96\x88"); /* █ */
                else if (top)         mvprintw(y, x, "\xe2\x96\x80"); /* ▀ */
                else                  mvprintw(y, x, "\xe2\x96\x84"); /* ▄ */
            } else {
                if (top && bot)       mvaddch(y, x, '#');
                else if (top)         mvaddch(y, x, '^');
                else                  mvaddch(y, x, '_');
            }
        }
    }
}

/** 2D grid ******************************************************************/
typedef char T;
typedef struct { int w, h, wh; T a[0]; } Mat;

Mat *m_create(int w, int h) {
    int wh = w * h;
    Mat *p = malloc(sizeof(Mat) + sizeof(T) * wh);
    p->w = w; p->h = h; p->wh = wh;
    memset(p->a, 0, sizeof(T) * wh);
    return p;
}
static inline bool m_in(Mat *p, int x, int y) { return x>=0 && x<p->w && y>=0 && y<p->h; }
static inline T m_get(Mat *p, int x, int y) { return m_in(p,x,y) ? p->a[y*p->w+x] : 0; }
static inline void m_put(Mat *p, int x, int y, T v) { if (m_in(p,x,y)) p->a[y*p->w+x]=v; }
static inline void m_cprow(Mat *p, int dst, int src) {
    memcpy(&p->a[dst*p->w], &p->a[src*p->w], sizeof(T)*p->w);
}
static uint32_t m_sum(Mat *p) {
    uint32_t s=0; int i;
    for (i=0; i<p->wh; i++) s+=p->a[i];
    return s;
}
static uint32_t m_adj8(Mat *p, int x, int y) {
    return m_get(p,x-1,y-1)+m_get(p,x,y-1)+m_get(p,x+1,y-1)+
           m_get(p,x-1,y)+m_get(p,x+1,y)+
           m_get(p,x-1,y+1)+m_get(p,x,y+1)+m_get(p,x+1,y+1);
}
static void mat_to_fb(Mat *m) {
    int x, y;
    for (y=0; y<m->h && y<g_fb.h; y++)
        for (x=0; x<m->w && x<g_fb.w; x++)
            if (m_get(m,x,y)) fb_set(x,y,1);
}

/** value noise **************************************************************/
static double vnoise_grid[64][64];
static void vnoise_seed() {
    int i, j;
    for (j=0; j<64; j++) for (i=0; i<64; i++) vnoise_grid[j][i]=urand();
}
static double lerp(double a, double b, double t) { return a+t*(b-a); }
static double smooth(double t) { return t*t*(3-2*t); }
static double vnoise2d(double x, double y) {
    double sx=fmod(fabs(x),63.0), sy=fmod(fabs(y),63.0);
    int ix=(int)sx, iy=(int)sy;
    double fx=smooth(sx-ix), fy=smooth(sy-iy);
    return lerp(lerp(vnoise_grid[iy][ix],vnoise_grid[iy][ix+1],fx),
                lerp(vnoise_grid[iy+1][ix],vnoise_grid[iy+1][ix+1],fx),fy);
}

/** breakout *****************************************************************/
typedef struct { Mat *m; int w,h; double bx,by,vx,vy; } Breakout;

static void bk_fill(Breakout *p) {
    vnoise_seed();
    int mg=max_i(3, min_i(p->w,p->h)/8);
    double sc=randb(4,8), th=randb(0.25,0.40);
    int i, j;
    for (j=mg; j<p->h-mg; j++)
        for (i=mg; i<p->w-mg; i++)
            m_put(p->m, i, j, vnoise2d(i/sc,j/sc)>th);
}
void *breakout_create(int w, int h) {
    Breakout *p = malloc(sizeof(Breakout));
    p->w=w; p->h=h; p->m=m_create(w,h);
    p->bx=w/2.0; p->by=h/2.0;
    double a=randb(0.3, PI/2-0.6);
    if (urand()>0.5) a=-a;
    p->vx=cos(a); p->vy=sin(a);
    bk_fill(p);
    return p;
}
void breakout_destroy(void *g) { Breakout *p=g; free(p->m); free(p); }
static void bk_nudge(Breakout *p) {
    double da=randm(0,0.08), c=cos(da), s=sin(da);
    double nvx=p->vx*c-p->vy*s, nvy=p->vx*s+p->vy*c;
    if (fabs(nvx)<0.15||fabs(nvy)<0.15) return;
    p->vx=nvx; p->vy=nvy;
}
int breakout_iter(void *g) {
    Breakout *p=g;
    p->bx+=p->vx; p->by+=p->vy;
    if (p->bx<=0||p->bx>=p->w-1) { p->vx=-p->vx; p->bx=p->bx<=0?0:p->w-1; bk_nudge(p); }
    if (p->by<=0||p->by>=p->h-1) { p->vy=-p->vy; p->by=p->by<=0?0:p->h-1; bk_nudge(p); }
    int ix=clamp_i((int)round(p->bx),0,p->w-1), iy=clamp_i((int)round(p->by),0,p->h-1);
    if (m_get(p->m,ix,iy)) {
        m_put(p->m,ix,iy,0);
        bool hx=m_get(p->m,ix-(p->vx>0?1:-1),iy);
        bool hy=m_get(p->m,ix,iy-(p->vy>0?1:-1));
        if (!hx) p->vx=-p->vx;
        if (!hy) p->vy=-p->vy;
        if (hx&&hy) { p->vx=-p->vx; p->vy=-p->vy; }
        bk_nudge(p);
    }
    if (m_sum(p->m)==0) bk_fill(p);
    return 0;
}
int breakout_draw(void *g) {
    Breakout *p=g;
    mat_to_fb(p->m);
    fb_set(clamp_i((int)round(p->bx),0,p->w-1), clamp_i((int)round(p->by),0,p->h-1), 1);
    return 0;
}

/** life *********************************************************************/
void *life_create(int w, int h) {
    Mat *m=m_create(w,h); int i,j;
    for (j=0;j<h;j++) for (i=0;i<w;i++) m_put(m,i,j,urand()<=0.10);
    return m;
}
void life_destroy(void *g) { free(g); }
int life_iter(void *g) {
    Mat *m=g;
    T *snap=malloc(m->wh); memcpy(snap,m->a,m->wh);
    int i,j;
    for (j=0;j<m->h;j++) for (i=0;i<m->w;i++) {
        int n=m_adj8(m,i,j); T cur=snap[j*m->w+i];
        m_put(m,i,j, cur?(n==2||n==3):(n==3));
    }
    if (urand()<0.02) {
        int cx=(int)randb(5,m->w-10), cy=(int)randb(5,m->h-10);
        m_put(m,cx+1,cy,1); m_put(m,cx+2,cy,1);
        m_put(m,cx,cy+1,1); m_put(m,cx+1,cy+1,1); m_put(m,cx+1,cy+2,1);
    }
    free(snap);
    return 0;
}
int life_draw(void *g) { mat_to_fb(g); return 0; }

/** rule30 *******************************************************************/
typedef struct { Mat *m; int w,h; } RuleX;
void *rulex_create(int w, int h) {
    RuleX *p=malloc(sizeof(RuleX)); p->w=w; p->h=h;
    p->m=m_create(w,h); m_put(p->m,w/2,h-1,1);
    return p;
}
void rulex_destroy(void *g) { RuleX *p=g; free(p->m); free(p); }
int rulex_iter(void *g) {
    RuleX *p=g; int x,y;
    for (y=0;y<p->h-1;y++) m_cprow(p->m,y,y+1);
    y=p->h-2;
    for (x=0;x<p->w;x++) {
        int a=m_get(p->m,x-1,y)*4+m_get(p->m,x,y)*2+m_get(p->m,x+1,y);
        m_put(p->m,x,p->h-1,a>=1&&a<=4);
    }
    return 0;
}
int rulex_draw(void *g) { RuleX *p=g; mat_to_fb(p->m); return 0; }

/** body3 ********************************************************************/
#define BODY_NUM 3
#define TRACE_NUM 100
typedef struct {
    int times,head,w,h;
    Vec2 pt[BODY_NUM][TRACE_NUM], v[BODY_NUM];
    double energy0;
} Body3;

static double b3_energy(Body3 *p) {
    int hd=p->head,i,j; double ke=0,pe=0;
    for (i=0;i<BODY_NUM;i++) ke+=0.5*v2abs2(p->v[i]);
    for (i=0;i<BODY_NUM-1;i++) for (j=i+1;j<BODY_NUM;j++) {
        double d=v2abs(v2sub(p->pt[i][hd],p->pt[j][hd]));
        if (d>0.01) pe-=1.0/d;
    }
    return ke+pe;
}
void *body3_create(int w, int h) {
    Body3 *p=malloc(sizeof(Body3));
    p->w=w; p->h=h; p->times=p->head=0;
    Vec2 ctr=v2(w/2.0,h/2.0); int i;
    p->pt[0][0]=ctr;
    for (i=1;i<BODY_NUM;i++)
        p->pt[i][0]=v2add(ctr, v2polar(min_i(w,h)*0.15, (2*PI*i)/BODY_NUM+randm(0,0.5)));
    Vec2 cm=v2zero();
    for (i=0;i<BODY_NUM;i++) cm=v2add(cm,p->pt[i][0]);
    cm=v2scale(cm,1.0/BODY_NUM);
    Vec2 sh=v2sub(ctr,cm);
    for (i=0;i<BODY_NUM;i++) p->pt[i][0]=v2add(p->pt[i][0],sh);
    p->v[0]=v2zero();
    for (i=1;i<BODY_NUM;i++) { p->v[i]=v2(randm(0,0.3),randm(0,0.3)); p->v[0]=v2sub(p->v[0],p->v[i]); }
    p->energy0=b3_energy(p);
    return p;
}
void body3_destroy(void *g) { free(g); }
int body3_iter(void *g) {
    Body3 *p=g;
    int old=p->head, nw=(old+1)%TRACE_NUM, i, j;
    Vec2 f[BODY_NUM]; for (i=0;i<BODY_NUM;i++) f[i]=v2zero();
    for (i=0;i<BODY_NUM-1;i++) for (j=i+1;j<BODY_NUM;j++) {
        Vec2 d=v2sub(p->pt[j][old],p->pt[i][old]);
        double d2=v2abs2(d), s2=4.0;
        Vec2 fij=v2scale(d, 1.0/((d2+s2)*sqrt(d2+s2)));
        f[i]=v2add(f[i],fij); f[j]=v2sub(f[j],fij);
    }
    for (i=0;i<BODY_NUM;i++) { p->v[i]=v2add(p->v[i],f[i]); p->pt[i][nw]=v2add(p->pt[i][old],p->v[i]); }
    Vec2 cm=v2zero();
    for (i=0;i<BODY_NUM;i++) cm=v2add(cm,p->pt[i][nw]);
    cm=v2scale(cm,1.0/BODY_NUM);
    Vec2 sh=v2sub(v2(p->w/2.0,p->h/2.0),cm);
    for (i=0;i<BODY_NUM;i++) p->pt[i][nw]=v2add(p->pt[i][nw],sh);
    Vec2 mv=v2zero();
    for (i=0;i<BODY_NUM;i++) mv=v2add(mv,p->v[i]);
    mv=v2scale(mv,1.0/BODY_NUM);
    for (i=0;i<BODY_NUM;i++) p->v[i]=v2sub(p->v[i],mv);
    double e=b3_energy(p);
    if (e>p->energy0*2.0) { double r=sqrt(fabs(p->energy0/e)); for (i=0;i<BODY_NUM;i++) p->v[i]=v2scale(p->v[i],r); }
    p->head=nw; p->times++;
    return 0;
}
int body3_draw(void *g) {
    Body3 *p=g; int hd=p->head,i,j;
    for (i=0;i<BODY_NUM;i++) {
        int ntr=min_i(p->times,TRACE_NUM-1);
        for (j=0;j<ntr;j++) {
            int k=(hd+TRACE_NUM-j)%TRACE_NUM, k1=(hd+TRACE_NUM-j-1)%TRACE_NUM;
            fb_line(p->pt[i][k],p->pt[i][k1]);
        }
        fb_pt(p->pt[i][hd]);
    }
    return 0;
}

/** square *******************************************************************/
#define VERTEX_NUM 5
typedef struct { int w,h; Vec2 vertex[VERTEX_NUM],origin,rot; } Sqr;
void *sqr_create(int w, int h) {
    Sqr *p=malloc(sizeof(Sqr)); p->w=w; p->h=h;
    p->origin=v2(w/2.0,h/2.0);
    double len=min_i(w,h)/3.0;
    Vec2 rotn=v2polar(1,(2*PI)/VERTEX_NUM);
    p->vertex[0]=v2(len,0);
    int i; for (i=1;i<VERTEX_NUM;i++) p->vertex[i]=v2mul(rotn,p->vertex[i-1]);
    p->rot=v2polar(1,0.01);
    return p;
}
void sqr_destroy(void *g) { free(g); }
int sqr_iter(void *g) { Sqr *p=g; int i; for (i=0;i<VERTEX_NUM;i++) p->vertex[i]=v2mul(p->rot,p->vertex[i]); return 0; }
int sqr_draw(void *g) {
    Sqr *p=g; int i;
    for (i=0;i<VERTEX_NUM;i++) fb_line(v2add(p->origin,p->vertex[i]),v2add(p->origin,p->vertex[(i+1)%VERTEX_NUM]));
    for (i=0;i<VERTEX_NUM;i++) fb_pt(v2add(p->origin,p->vertex[i]));
    return 0;
}

/** game framework ***********************************************************/
typedef void *(*create_fn)(int,int);
typedef int (*step_fn)(void *);
typedef void (*free_fn)(void *);
typedef struct { const char *name; create_fn create; step_fn iter; step_fn draw; free_fn destroy; int freq; const char *desc; } Game;

Game gamelst[] = {
    {"square",   sqr_create,     sqr_iter,     sqr_draw,     sqr_destroy,     30, "rotating polygon"},
    {"body3",    body3_create,   body3_iter,   body3_draw,   body3_destroy,   30, "three-body simulation"},
    {"rulex",    rulex_create,   rulex_iter,   rulex_draw,   rulex_destroy,    1, "rule30 cellular automaton"},
    {"life",     life_create,    life_iter,    life_draw,    life_destroy,     8, "Conway's Game of Life"},
    {"breakout", breakout_create,breakout_iter,breakout_draw,breakout_destroy,30, "breakout ball animation"},
    {NULL},
};
#define GAME_COUNT (sizeof(gamelst)/sizeof(gamelst[0])-1)

Game *select_game(const char *name) {
    int i; for (i=0;gamelst[i].name;i++) if (strcasecmp(name,gamelst[i].name)==0) return &gamelst[i];
    return NULL;
}

static void print_help(const char *prog) {
    fprintf(stderr,"Usage: %s [OPTIONS] [MODE]\n\nTerminal screensaver. Press any key to exit.\n\nModes:\n",prog);
    int i; for (i=0;gamelst[i].name;i++) fprintf(stderr,"  %-12s %s\n",gamelst[i].name,gamelst[i].desc);
    fprintf(stderr,"  %-12s randomly switch between modes\n","random");
    fprintf(stderr,"\nOptions:\n  --fancy      Unicode half-block rendering\n  --compat     ASCII rendering (default)\n  -h, --help   show this help\n");
}

static volatile sig_atomic_t g_quit = 0;
static void sig_handler(int sig) { (void)sig; g_quit=1; }

int main(int argc, char **argv) {
    srand(time(NULL));
    const char *name="breakout"; int i;
    for (i=1;i<argc;i++) {
        if (!strcmp(argv[i],"-h")||!strcmp(argv[i],"--help")) { print_help(argv[0]); return 0; }
        else if (!strcmp(argv[i],"--fancy")) g_render_mode=RENDER_FANCY;
        else if (!strcmp(argv[i],"--compat")) g_render_mode=RENDER_COMPAT;
        else if (argv[i][0]!='-') name=argv[i];
        else { fprintf(stderr,"Unknown option: %s\nTry --help\n",argv[i]); return 1; }
    }
    bool random_mode=!strcasecmp(name,"random");
    if (!random_mode && !select_game(name)) {
        fprintf(stderr,"Unknown mode: '%s'\nAvailable:",name);
        for (i=0;gamelst[i].name;i++) fprintf(stderr," %s",gamelst[i].name);
        fprintf(stderr," random\nTry --help\n"); return 1;
    }
    signal(SIGINT,sig_handler); signal(SIGTERM,sig_handler);

    setlocale(LC_ALL,"");
    initscr(); noecho(); curs_set(FALSE); nodelay(stdscr,TRUE);
    int scr_w, scr_h;
    getmaxyx(stdscr, scr_h, scr_w);
    int lw=scr_w, lh=scr_h*2;
    fb_init(lw,lh);

    int rf=0, rl=0; Game *game=NULL; void *g=NULL;
    for (;;) {
        if (!game||(random_mode && rf>=rl)) {
            if (g&&game) { game->destroy(g); g=NULL; }
            game=random_mode?&gamelst[rand()%GAME_COUNT]:select_game(name);
            g=game->create(lw,lh);
            rf=0; rl=game->freq*(int)randb(15,30);
        }
        if (getch()!=ERR||g_quit) break;
        fb_clear(); game->iter(g); game->draw(g);
        erase(); fb_blit(scr_w,scr_h); refresh();
        usleep(1000000/game->freq); rf++;
    }
    if (g&&game) game->destroy(g);
    fb_free(); endwin();
    return 0;
}
