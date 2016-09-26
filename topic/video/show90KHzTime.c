#include <stdio.h>


typedef unsigned long long UINT64;
#define TIMESTRLEN (2 + 2 + 2 + 4 + 3 + 1)
char *show90KHzTime(char *s, UINT64 pts) {
    sprintf(s, "%02lld:%02lld:%02lld.%03lld",
            (pts / 90000) / 3600,
            ((pts / 90000) % 3600) / 60,
            (pts / 90000) % 60,
            (pts% 90000 * 1000) / 90000);
    return s;
}


#define SHOW(t) printf("%10lld: %s\n", t, show90KHzTime(s, t));
int main() {
    char s[TIMESTRLEN];
    UINT64 i;
    UINT64 t;
    for (i = 0; i < 100; i++) {
        t = 1000 * i;
        SHOW(t);
    }
    SHOW(0xffffffffll);
    SHOW(0x1ffffffffll);
    return 0;
}
