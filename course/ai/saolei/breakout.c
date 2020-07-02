#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

typedef char Elem;
typedef struct {
    int w, h, wh;
    Elem a[0];
} Mat;

Mat *m_create(int w, int h) {
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

int main() {
    initscr();
    noecho();
    curs_set(FALSE);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // stdscr from initscr, get max_y, max_x

    Mat *m = m_create(max_x, max_y);
    int block = 8;
    int i, j;
    for (j = block; j != m->h - block; j++) {
        for (i = block; i != m->w - block; i++) {
            put(m, i, j, 1);
        }
    }

    int cx = 1, cy = 1;
    int x = 0, y = 0;
    while (1) {
        clear();
        mvprintw(y, x, "O");
        for (j = block; j != m->h - block; j++) {
            for (i = block; i != m->w - block; i++) {
                if (idx(m, i, j)) {
                    mvprintw(j, i, "X");
                }
            }
        }

        refresh();
        usleep(30 * 1000);
        x = x + cx;
        y = y + cy;
        if (x == 0 || x == max_x) {
            cx = -cx;
        }
        if (y == 0 || y == max_y) {
            cy = -cy;
        }
        if (idx(m, x, y)) {
            put(m, x, y, 0);
            if (idx(m, x - 1, y) || idx(m, x + 1, y)) {
                put(m, x - 1, y, 0);
                put(m, x + 1, y, 0);
                cx = -cx;
            }
            if (idx(m, x, y - 1) || idx(m, x, y + 1)) {
                put(m, x, y - 1, 0);
                put(m, x, y + 1, 0);
                cy = -cy;
            }
        }
        if (m_sum(m) == 0) {
            break;
        }
    }
    // mvprintw(0, 0, "Hello, Ncurse!");
    // refresh();
    // sleep(1);

    endwin();
    free(m);

    return 0;
}
