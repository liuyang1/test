#include <string.h>
#include <stdio.h>

#define TIMESTRLEN (2 + 2 + 2 + 6 + 3 + 1)

typedef unsigned long long UINT64;

char *showScaledTime(char *s, UINT64 pts, UINT64 scale) {
    UINT64 seconds = pts / scale;
    UINT64 ms = ((pts % scale) * 1000 * 1000) / scale;
    snprintf(s, TIMESTRLEN, "%02lld:%02lld:%02lld.%06lld",
             seconds / 3600,
             (seconds % 3600) / 60,
             seconds % 60,
             ms);
    return s;
}

char *show90KHzTime_new(char *s, UINT64 pts) {
    return showScaledTime(s, pts, 90 * 1000);
}

char *showMsTime_new(char *s, UINT64 pts) {
    return showScaledTime(s, pts, 1000 * 1000);
}


char *show90KHzTime(char *s, UINT64 pts) {
    snprintf(s, TIMESTRLEN, "%02lld:%02lld:%02lld.%03lld",
            (pts / 90000) / 3600,
            ((pts / 90000) % 3600) / 60,
            (pts / 90000) % 60,
            // add 45 for round up to nearest decimal time
            ((pts + 45) % 90000 * 1000) / 90000);
    return s;
}

char *showMsTime(char *s, UINT64 pts) {
    UINT64 seconds = pts / (1000 * 1000);
    UINT64 ms = pts % (1000 * 1000);
    snprintf(s, TIMESTRLEN, "%02lld:%02lld:%02lld.%06lld",
             seconds / 3600,
             (seconds % 3600) / 60,
             seconds % 60,
             ms);
    return s;
}

#define TEST_SHOWTIME(func, pts) \
    test_showTime(#func, func, pts)

int test_showTime(char *funcname, char *(*f)(char *, UINT64),
                  UINT64 pts) {
    char s[TIMESTRLEN];
    f(s, pts);
    printf("%s(%lld %llx)=%s len=%lu\n", funcname, pts, pts, s, strlen(s));
    return 0;
}

int main() {
    printf("TIMESTRLEN=%d\n", TIMESTRLEN);
    TEST_SHOWTIME(show90KHzTime, 1000);
    TEST_SHOWTIME(show90KHzTime_new, 1000);
    TEST_SHOWTIME(showMsTime, 1000);
    TEST_SHOWTIME(showMsTime_new, 1000);

    TEST_SHOWTIME(show90KHzTime, 0x16c8c38);
    TEST_SHOWTIME(show90KHzTime_new, 0x16c8c38);
    TEST_SHOWTIME(showMsTime, 0x16c8c38);
    TEST_SHOWTIME(showMsTime_new, 0x16c8c38);

    TEST_SHOWTIME(showMsTime_new, 0x116c8c38ull << 32);
    return 0;
}
