#include <stdio.h>
#include <string.h>
#define P (1000 * 1000 * 1000 + 7)
#define N (10000 + 1)
#if 0
int checkRecord(int n, int a_cnt, int cl) {
    if (n == 0) {
        return 1;
    }
    int p = checkRecord(n - 1, a_cnt, 0);
    int a = 0;
    if (a_cnt <= 0) {
        a = checkRecord(n - 1, a_cnt + 1, 0);
    }
    int l = 0;
    if (cl <= 1) {
        l = checkRecord(n - 1, a_cnt, cl + 1);
    }
    return (p + a + l) % P;
}

void initTbl() {
}

void showTbl() {
}

#else
static int a[N][2][3]; // N * a_cnt={0,1} * cl={0,1,2}
static int max_n = 0;
static int init = 0;
static inline void initTbl() {
    memset(a, 0x00, sizeof(a));
    a[0][0][0] = 1;
    a[0][0][1] = 1;
    a[0][0][2] = 1;
    a[0][1][0] = 1;
    a[0][1][1] = 1;
    a[0][1][2] = 1;
}

static inline int addm(int a, int b) {
    return (a + b) % P;
}

static inline int addm3(int a, int b, int c) {
    return ((a + b) % P + c) % P;
}

int checkRecord(int n) {
    if (init == 0) {
        initTbl();
        init = 1;
    }
    if (n > max_n) {
        int i;
        for (i = max_n; i < n; i++) {
            a[i + 1][0][0] = addm3(a[i][0][0], a[i][1][0], a[i][0][1]);
            a[i + 1][0][1] = addm3(a[i][0][0], a[i][1][0], a[i][0][2]);
            a[i + 1][0][2] = addm(a[i][0][0], a[i][1][0]);
            a[i + 1][1][0] = addm(a[i][1][0], a[i][1][1]);
            a[i + 1][1][1] = addm(a[i][1][0], a[i][1][2]);
            a[i + 1][1][2] = a[i][1][0];
        }
    }
    return a[n][0][0];
}

void showTbl() {
    int i;
    for (i = 0; i != 10; i++) {
        printf("%d %d %d\t%d %d %d\n",
               a[i][0][0], a[i][0][1], a[i][0][2],
               a[i][1][0], a[i][1][1], a[i][1][2]);
    }
}

#endif

int main() {
    // initTbl();
    printf("%d\n", P);
    // int i;
    // for (i = 0; i != 30; i++) {
    //     printf("%d\n", checkRecord(i, 0, 0));
    // }
    printf("%d\n", checkRecord(29));
    showTbl();
    return 0;
}
