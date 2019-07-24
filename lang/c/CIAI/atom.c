#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
#include "assert.h"
#include "mem.h"
#include "atom.h"

#define EXERCISE_3_4
#define NELEMS(x) ((sizeof(x)) / sizeof(((x)[0])))

static unsigned long scatter[] = {
    2078917053, 143302914, 1027100827, 1953210302, 755253631, 2002600785,
    1405390230, 45248011, 1099951567, 433832350, 2018585307, 438263339,
    813528929, 1703199216, 618906479, 573714703, 766270699, 275680090,
    1510320440, 1583583926, 1723401032, 1965443329, 1098183682, 1636505764,
    980071615, 1011597961, 643279273, 1315461275, 157584038, 1069844923,
    471560540, 89017443, 1213147837, 1498661368, 2042227746, 1968401469,
    1353778505, 1300134328, 2013649480, 306246424, 1733966678, 1884751139,
    744509763, 400011959, 1440466707, 1363416242, 973726663, 59253759,
    1639096332, 336563455, 1642837685, 1215013716, 154523136, 593537720,
    704035832, 1134594751, 1605135681, 1347315106, 302572379, 1762719719,
    269676381, 774132919, 1851737163, 1482824219, 125310639, 1746481261,
    1303742040, 1479089144, 899131941, 1169907872, 1785335569, 485614972,
    907175364, 382361684, 885626931, 200158423, 1745777927, 1859353594,
    259412182, 1237390611, 48433401, 1902249868, 304920680, 202956538,
    348303940, 1008956512, 1337551289, 1953439621, 208787970, 1640123668,
    1568675693, 478464352, 266772940, 1272929208, 1961288571, 392083579,
    871926821, 1117546963, 1871172724, 1771058762, 139971187, 1509024645,
    109190086, 1047146551, 1891386329, 994817018, 1247304975, 1489680608,
    706686964, 1506717157, 579587572, 755120366, 1261483377, 884508252,
    958076904, 1609787317, 1893464764, 148144545, 1415743291, 2102252735,
    1788268214, 836935336, 433233439, 2055041154, 2109864544, 247038362,
    299641085, 834307717, 1364585325, 23330161, 457882831, 1504556512,
    1532354806, 567072918, 404219416, 1276257488, 1561889936, 1651524391,
    618454448, 121093252, 1010757900, 1198042020, 876213618, 124757630,
    2082550272, 1834290522, 1734544947, 1828531389, 1982435068, 1002804590,
    1783300476, 1623219634, 1839739926, 69050267, 1530777140, 1802120822,
    316088629, 1830418225, 488944891, 1680673954, 1853748387, 946827723,
    1037746818, 1238619545, 1513900641, 1441966234, 367393385, 928306929,
    946006977, 985847834, 1049400181, 1956764878, 36406206, 1925613800,
    2081522508, 2118956479, 1612420674, 1668583807, 1800004220, 1447372094,
    523904750, 1435821048, 923108080, 216161028, 1504871315, 306401572,
    2018281851, 1820959944, 2136819798, 359743094, 1354150250, 1843084537,
    1306570817, 244413420, 934220434, 672987810, 1686379655, 1301613820,
    1601294739, 484902984, 139978006, 503211273, 294184214, 176384212,
    281341425, 228223074, 147857043, 1893762099, 1896806882, 1947861263,
    1193650546, 273227984, 1236198663, 2116758626, 489389012, 593586330,
    275676551, 360187215, 267062626, 265012701, 719930310, 1621212876,
    2108097238, 2026501127, 1865626297, 894834024, 552005290, 1404522304,
    48964196, 5816381, 1889425288, 188942202, 509027654, 36125855,
    365326415, 790369079, 264348929, 513183458, 536647531, 13672163,
    313561074, 1730298077, 286900147, 1549759737, 1699573055, 776289160,
    2143346068, 1975249606, 1136476375, 262925046, 92778659, 1856406685,
    1884137923, 53392249, 1735424165, 1602280572
};

static struct atom {
    struct atom *link;
    unsigned long hash;
    int len;
#ifdef EXERCISE_3_4
    char str[1]; // This way save one pointer (32bit/64bit), and one char
    // And it save one time indirect access by pointer
#else
    char *str;
#endif
} *buckets[2039];
/** Exer 3.1, Improve from 2048 to 2039
 * check simple_hash_string function, when buckets length is 2048;
 * The algo means only last 20 char have effect.(For real test, it's same when length>=11chars)
 * When buckets is 2039, the collision always happen when length>=64chars
 */


const char *Atom_string(const char *str) {
    assert(str);
    return Atom_new(str, strlen(str));
}

const char *Atom_int(long n) {
    // why use 43?
    // 43 = 128 / 3 + 1
    // so for 128bit number, we could 43 bytes to store it.
    char str[43];
    char *s = str + sizeof(str);
    unsigned long m;

    // why need do this?
    // unsigned division and modulus are well defined. so use unsigned long.
    if (n == LONG_MIN) {
        m = LONG_MAX + 1ULL; // special case to avoid overflow
    } else if (n < 0) {
        m = -n;
    } else {
        m = n;
    }
    do {
        *--s = m % 10 + '0';
    } while ((m /= 10) > 0);
    if (n < 0) {
        *--s = '-';
    }
    return Atom_new(s, (str + sizeof(str)) - s);
}

static unsigned long simple_hash_string(const char *str, int len) {
    int i;
    unsigned long h;
    for (i = 0, h = 0; i != len; i++) {
        h = (h << 1) + scatter[(unsigned char)str[i]];
    }
    return h;
}

static unsigned long my_hash_string(const char *str, int len) {
    int i;
    unsigned long h;
    for (i = h = 0; i != len; i++) {
        h = h * 97 + str[i];
    }
    return h;
}

static unsigned long dumb_hash(const char *str, int len) {
    return 0; // so we could debug on chain-list
}

// #define HASH_FUNC   dumb_hash
// #define HASH_FUNC   simple_hash_string
#define HASH_FUNC   my_hash_string

const char *Atom_new(const char *str, int len) {
    unsigned long h;
    int i;
    struct atom *p;

    assert(str);
    assert(len >= 0);
    h = HASH_FUNC(str, len);
    // h &= NELEMS(buckets) - 1; // only work at NELEMS == 2 ^ n
    h %= NELEMS(buckets);
    /** Exer 3.3 Why don't use strncmp here? */
    /* - strcmp, but not strncmp
     * - we already know & compared string length, so we don't need check
     *   strings' end now.
     *   p->str[i] == '\0' || str[i] == '\0'
     * - I think it's okay, if use `memcmp`
     */
    /** chaining-list hash */
    for (p = buckets[h]; p; p = p->link) {
        if (p->str == str) {
            return p->str;
        }
        /** Exer 3.5 add hash in entry to save comparasion */
        if (len == p->len && h == p->hash) {
            for (i = 0; i < len && p->str[i] == str[i];) {
                i++;
            }
            if (i == len) {
                // find stored atom
                return p->str;
            }
        }
    }
    // for atom struct and string
    unsigned int size;
#ifdef EXERCISE_3_4
    size = sizeof(*p) + sizeof(char) * len;
#else
    size = sizeof(*p) + sizeof(char) * (len + 1);
#endif
    // printf("size=%d\n", size);
    p = ALLOC(size);
    p->len = len;
    p->hash = h;
#ifndef EXERCISE_3_4
    p->str = (char *)(p + 1); // next to bucket, it's str part.
#endif
    if (len > 0) {
        memcpy(p->str, str, len);
    }
    p->str[len] = '\0';
    p->link = buckets[h];
    buckets[h] = p;
    return p->str;
}

int Atom_length(const char *str) {
    assert(str);
    struct atom *p;
    // Exer 3.6 get length first, to avoid loop on whole buckets
    int len = strlen(str);
    unsigned long h;
    h = HASH_FUNC(str, len);
    h %= NELEMS(buckets);
    for (p = buckets[h]; p; p = p->link) {
        if (p->str == str) {
            return p->len;
        }
    }
    assert(0);
    return 0;
}

void Atom_reset() {
    struct atom *p, *next;
    unsigned int i;
    for (i = 0; i != NELEMS(buckets); i++) {
        for (p = buckets[i]; p; p = next) {
            next = p->link;
            FREE(p);
        }
        buckets[i] = NULL;
    }
}

void Atom_debug() {
    struct atom *p;
    unsigned int i, j, slot;
    unsigned int *stat = ALLOC(sizeof(unsigned int) * NELEMS(buckets));
    for (i = j = slot = 0; i != NELEMS(buckets); i++) {
        for (p = buckets[i], slot = 0; p; p = p->link) {
            slot++;
            j++;
#if 1
            printf("i=%d p=%p len=%u '%s'\n", i, p, p->len, p->str);
#endif
        }
        stat[i] = slot;
    }
    double var = 0, avg = j / (NELEMS(buckets) + 0.);
    for (i = 0; i != NELEMS(buckets); i++) {
        var += (stat[i] - avg) * (stat[i] - avg);
    }
    var /= NELEMS(buckets);
    printf("atom num=%u all=%lu avg=%f var=%f\n", j, NELEMS(buckets), avg, var);
    FREE(stat);
}

int Atom_number() {
    struct atom *p;
    unsigned int i, cnt;
    for (i = cnt = 0; i != NELEMS(buckets); i++) {
        for (p = buckets[i]; p; p = p->link) {
            cnt++;
        }
    }
    return cnt;
}

void Atom_free(const char *str) {
    assert(str);
    int len = strlen(str);
    unsigned long h;
    h = HASH_FUNC(str, len);
    h %= NELEMS(buckets);
#if 1
    struct atom *p, *last;
    p = buckets[h];
    if (p->str == str) {
        buckets[h] = p->link;
        FREE(p);
        return;
    }
    for (; p; p = p->link) {
        if (p->str == str) {
            last->link = p->link;
            FREE(p);
            return;
        }
        last = p;
    }
#else
    struct atom *p, **pp;
    p = buckets[h];
    for (pp = &p; p; pp = &p, p = p->link) {
        if (p->str == str) {
            *pp = p->link;
            FREE(p);
            return;
        }
    }
#endif
}

void Atom_vload(const char *str, ...) {
    va_list ap;
    va_start(ap, str);
    for (; str; str = va_arg(ap, const char *)) {
        Atom_string(str);
    }
    va_end(ap);
}

void Atom_aload(const char *str[]) {
    for (; *str != NULL; str++) {
        Atom_string(*str);
    }
}
