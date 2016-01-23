/*
 * Rabin-Karp Algo
 * But with BAD hash algo: simple sum, then roll
 */
#include "leet.h"

const unsigned int prime = ((1u << 31) - 1);
const unsigned int base = 1;
int map(char c) {
    return c;
}

typedef struct {
    unsigned long long r;
    char *s;
    int chunksz;
    int idx;
    unsigned long long pwr;
} Hash;

void initHash(Hash *hs, char *s, int chunksz) {
    unsigned long long r = 0;
    hs->pwr = 0;
    int i;
    LOG("init hash %s chunksz=%d\n", s, chunksz);
    for (i = 0; i != chunksz; i++) {
        r = (r * base + map(s[i])) % prime;
        if (hs->pwr == 0) {
            hs->pwr = 1;
        } else {
            hs->pwr = (hs->pwr * base) % prime;
        }
        LOG("init hash c=%c r=%llu pwr=%lld\n", s[i], r, hs->pwr);
    }
    hs->s = s;
    hs->idx = 0;
    hs->r = r;
    hs->chunksz = chunksz;
}

unsigned long long getHash(Hash *hs) {
    return hs->r;
}

bool rollHash(Hash *hs) {
    int out = hs->idx;
    int in = hs->idx + hs->chunksz;
    if (hs->s[hs->idx] == '\0') {
        return false;
    }
    if (hs->s[in] != hs->s[out]) {
        unsigned long long r = base * (hs->r - hs->s[out] * hs->pwr) + hs->s[in];
        r %= prime;
        LOG("out %3d %c in %3d %c old %10llu new %10llu\n",
            out, hs->s[out], in, hs->s[in], hs->r, r);
        hs->r = r;
    }
    hs->idx++;
    return true;
}

int strStr(char *haystack, char *needle) {
    if (*needle == '\0') {
        return 0;
    }
    int sz0 = strlen(haystack), sz1 = strlen(needle);
    if (sz0 < sz1) {
        return -1;
    }
    Hash hs0, hs1;
    initHash(&hs0, haystack, sz1);
    initHash(&hs1, needle, sz1);
    int i;
    unsigned long long ptn = getHash(&hs1);
    unsigned long long val;
    for (i = 0; i != sz0 - sz1 + 1; i++) {
        val = getHash(&hs0);
        if (val == ptn) {
            LOG("index %d find match hash %llu\n", i, val);
            if (strncmp(haystack + i, needle, sz1) == 0) {
                return i;
            }
        }
        rollHash(&hs0);
    }
    return -1;
}

#define CASE(a, b, e) {int r = strStr(a, b); \
                       printf("%s %s %d ?= %d %s\n", a, b, r, e, expect(r == e));  }

int main() {
    CASE("", "", 0);
    CASE("abc", "", 0);
    CASE("abc", "a", 0);
    CASE("abc", "bc", 1);
    CASE("here is a simple example", "example", 17);
    CASE("karp riachard; rabin michael", "rabin", 15);
    CASE("abb", "abaaa", -1);
    CASE("abbbbbbbaabaa", "bbbbbbaa", 2);
    return 0;
}
