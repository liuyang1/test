#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int x, y;
} Pt;
typedef struct {
    int a, b, c;
} TwoPt;

int cmpPt(const void *u, const void *v) {
    const Pt *a = u, *b = v;
    if (a->x != b->x) {
        return a->x - b->x;
    }
    return a->y - b->y;
}
int cmpTwoPt(const void *a, const void *b) {
    const TwoPt *x = a, *y = b;
    if (x->a != y->a) {
        return x->a - y->a;
    }
    if (x->b != y->b) {
        return x->b - y->b;
    }
    if (x->c != y->c) {
        return x->c - y->c;
    }
    return 0;
}

static inline int gcd(int a, int b) {
    if (a == b) {
        return a;
    }
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    if (a % 2 == 0) {
        if (b % 2 == 0) {
            return gcd(a / 2, b / 2) * 2;
        } else {
            return gcd(a / 2, b);
        }
    } else {
        if (b % 2 == 0) {
            return gcd(a, b / 2);
        } else {
            if (a > b) {
                return gcd(a - b, b);
            } else {
                return gcd(b - a, a);
            }
        }
    }
}

static inline int sgn(int x) {
    return (x > 0) - (x < 0);
}

static inline int gcd3(int a, int b, int c) {
    // printf("%d,%d,%d\n", a, b, c);
    int r =  gcd(gcd(abs(a), abs(b)), abs(c));
    int sign = sgn(a);
    if (sign == 0) {
        sign = sgn(b);
    }
    if (sign == 0) {
        sign = sgn(c);
    }
    // printf("%d\n", r);
    return r * sign;
}

static inline TwoPt line(Pt d1, Pt d2) {
    int a = d2.y - d1.y, b = d1.x - d2.x, c = (d2.y - d1.y) * d1.x - (d2.x - d1.x) * d1.y;
    int g = gcd3(a, b, c);
    if (g == 0) {
        g = 1;
    }
    TwoPt r = {.a = a / g, .b = b / g, .c = c / g};
    return r;
}

static inline void showTwoPt(TwoPt t) {
    printf("%dx+%dy=%d\n", t.a, t.b, t.c);
}
static inline void showPt(Pt t) {
    printf("%d,%d\n", t.x, t.y);
}

static inline int deTriNum(int x, int n) {
    printf("deTriNum %d %d\n", x, n);
    int b, e, m;
    for (b = 0, e = n; b <= e;) {
        m = (b + e) / 2;
        int t = m * (m - 1) / 2;
        if (x == t) {
            return m;
        } else if (x > t) {
            b = m + 1;
        } else {
            e = m - 1;
        }
    }
    return 0;
}

/** sort (base, nmemb, size, compar)
 * map (head &&& length) . group . sort
 * uniq sorted list, stored in place
 * length list, stored in new memory
 */
size_t sortGroupCnt(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *),
                    size_t **opN) {
    printf("qsort base=%p nmemb=%zu size=%zu compar=%p opN=%p\n",
           base, nmemb, size, compar, opN);
    qsort(base, nmemb, size, compar);
    size_t *cnts = malloc(sizeof(size_t) * nmemb);
    printf("after qsort\n");
    void *last = NULL;
    size_t i, j, cnt;
    for (i = j = cnt = 0;  i!= nmemb; i++) {
        if (last != NULL && compar(last, base + i * size) == 0) {
            cnt++; // same
        } else {
            if (last != NULL) {
                memcpy(base + j * size, last, size), cnts[j] = cnt, j++;
            }
            last = base + i * size, cnt = 1;
        }
    }
    if (last != NULL) {
        memcpy(base + j * size, last, size), cnts[j] = cnt, j++;
    }
    if (opN) {
        *opN = cnts;
    } else {
        free(cnts);
    }
    return j;
}


int maxPoints(int **points, int pointsSize, int *pointsColSize) {
    if (points == NULL || pointsSize <= 1) {
        return pointsSize;
    }
    int nn = (pointsSize) * (pointsSize - 1) / 2;
    TwoPt *a = malloc(sizeof(TwoPt) * nn);
    int i, j, k;
    for (i = k = 0; i != pointsSize; i++) {
        Pt d1 = {.x = points[i][0], points[i][1]};
        for (j = i + 1; j != pointsSize; j++) {
            Pt d2 = {.x = points[j][0], points[j][1]};
            TwoPt r = line(d1, d2);
            showTwoPt(r);
            a[k++] = r;
        }
    }
    qsort(a, nn, sizeof(TwoPt), cmpTwoPt);
    TwoPt last = a[0];
    int mcnt = 1, cnt = 1;
    for (k = 1; k != nn; k++) {
        if (cmpTwoPt(&a[k], &last) == 0) {
            cnt++;
            if (cnt > mcnt) {
                mcnt = cnt;
            }
        } else {
            printf("cnt=%d\n", cnt);
            last = a[k];
            showTwoPt(last);
            cnt = 1;
        }
    }
    printf("%d\n", mcnt);
    int ret = deTriNum(mcnt, pointsSize);
    printf("%d\n", ret);
    free(a);
    return ret;
}

int unit0() {
    int pts[][2] = {{1, 1}, {3, 2}, {5, 3}, {4, 1}, {2, 3}, {1, 4}};
    int *points[] = {pts[0], pts[1], pts[2], pts[3], pts[4], pts[5]};
    int size = 0;
    int r = maxPoints(points, sizeof(points) / sizeof(points[0]), &size);
    printf("r=%d\n", r);
    return 0;
}

int unit1() {
    // int pts[][2] = {{1, 1}, {2,2 }, {3, 3}};
    // int *points[] = {pts[0], pts[1], pts[2]};
    // int pts[][2] = {{1, 1}, {2,2 }};
    // int *points[] = {pts[0], pts[1]};
    // int pts[][2] = {{1, 1}};
    // int *points[] = {pts[0]};
    // int pts[][2] = {{1, 1}, {1, 1}};
    // int *points[] = {pts[0], pts[1]};
    // int pts[][2] = {{1, 1}, {1,1 }, {1, 1}};
    // int *points[] = {pts[0], pts[1], pts[2]};
    // int pts[][2] = {{0, 0}, {1,1 }, {1, 1}};
    // int *points[] = {pts[0], pts[1], pts[2]};
    int pts[][2] = {{2, 2}, {1,1 }, {1, 1}};
    int *points[] = {pts[0], pts[1], pts[2]};
    int size = 0;
    int r = maxPoints(points, sizeof(points) / sizeof(points[0]), &size);
    printf("r=%d\n", r);
    return 0;
}
int unit2() {
    // int pts[][2] = {};
    // int pts[][2] = {{1,1 }};
    // int pts[][2] = {{1, 1}, {1,1 }};
    // int pts[][2] = {{2, 2}, {1,1 }};
    int pts[][2] = {{2, 2}, {1,1 }, {1, 1}};
    size_t *pn = NULL;
    size_t n = sortGroupCnt(pts, sizeof(pts) / sizeof(pts[0]), sizeof(pts[0]), cmpPt,
                            &pn);
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%d,%d\n", pts[i][0], pts[i][1]);
        if (pn != NULL) {
            printf("%zu\n", pn[i]);
        }
    }
    if (pn != NULL) {
        free(pn);
    }
    return 0;
}

int main() {
    // unit0();
    // unit1();
    unit2();
    return 0;
}
