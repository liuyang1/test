#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef unsigned long long UINT64;
typedef unsigned long UINT32;

#define TIMESTRLEN (2 + 2 + 2 + 3 + 3 + 1)
#define TIMESTAMPMAX    0x1ffffffffll

char *show90KHzTime(char *s, UINT64 pts) {
    if (pts > TIMESTAMPMAX) {
        return NULL;
    }
    sprintf(s, "%02lld:%02lld:%02lld.%03lld",
            (pts / 90000) / 3600,
            ((pts / 90000) % 3600) / 60,
            (pts / 90000) % 60,
            // add 45 for round up to nearest decimal time
            ((pts + 45) % 90000 * 1000) / 90000);
    return s;
}

char *show90KHzTimeHL(char *s, UINT32 ptshigh, UINT32 ptslow) {
    UINT64 t = ptshigh << 32;
    t = (t << 32) + ptslow;
    return show90KHzTime(s, t);
}

char *show90KHzTimeL(char *s, UINT32 ptslow) {
    return show90KHzTime(s, ptslow);
}

#define SHOW(t) printf("%10lld: %s\n", t, show90KHzTime(s, t));
#define CHECK_STR_EQ(a, b) {if (strcmp(a, b) != 0) { \
                                printf("\"%s\" ?= \"%s\" %s", a, b, "FAIL"); return false; }}
#define CHECK_UINT_EQ(a, b) {if (a != b) {printf("%d ?= %d FAIL", a, b); return false; }}
#define CHECK_PTR_EQ(a, b) {if (a != b) {printf("%p ?= %p FAIL", a, b); return false; }}

bool test_round() {
    char s[TIMESTRLEN];
    show90KHzTime(s, 1500);
    // 1500/90000 => 0.0166 -> 0.017
    CHECK_STR_EQ(s, "00:00:00.017");
    return true;
}

int main() {
    char s[TIMESTRLEN];
    UINT64 i;
    UINT64 t;
    for (i = 0; i < 100; i++) {
        t = 1000 * i;
        SHOW(t);
    }
    SHOW(0xffffffffll);
    SHOW(TIMESTAMPMAX);
    int len = strlen(show90KHzTime(s, TIMESTAMPMAX));
    CHECK_UINT_EQ(TIMESTRLEN, len + 1);
    char *p = show90KHzTime(s, TIMESTAMPMAX + 1);
    CHECK_PTR_EQ(p, NULL);

    UINT32 tl = 0xffffffffll;
    printf("%10ld: %s\n", tl, show90KHzTimeL(s, tl));
    printf("%10ld %10ld: %s\n", 1l, tl, show90KHzTimeHL(s, 1, tl));

    printf("wrong usage: %s %s\n", show90KHzTime(s, 1000), show90KHzTime(s, 2000));

    test_round();
    return 0;
}
