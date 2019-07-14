#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TIC {struct timespec start, stop; clock_gettime(CLOCK_MONOTONIC, &start);
#define TOC clock_gettime(CLOCK_MONOTONIC, &stop);                                         \
    printf("%lu.%lu - %lu.%lu\n", start.tv_sec, start.tv_nsec, stop.tv_sec, stop.tv_nsec); \
    printf("%lu sec %luusec\n", stop.tv_sec - start.tv_sec, stop.tv_nsec - start.tv_nsec);}

#include "atom.h"

#include "test.h"

bool testAtomSimple() {
    Atom_reset();
    const char *s = Atom_string("abc");
    ASSERT(strcmp(s, "abc") == 0);

    s = Atom_int(1234);
    ASSERT(strcmp(s, "1234") == 0);

    s = Atom_new("abcdef", 3);
    ASSERT(strcmp(s, "abc") == 0);

    int n = Atom_length(s);
    ASSERT(n == 3);
    return true;
}

bool testAtomEq() {
    const char *s0 = Atom_string("abc");
    const char *s1 = Atom_string("abc");
    EXPECT_PTR_EQ(s0, s1);

    const char *s2 = Atom_string("def");
    ASSERT(s0 != s2);
    return true;
}

int perf_atom() {
    FILE *fp = fopen("/home/ly/Hello.java", "r");
    char *p = NULL;
    size_t len = 0;
    ssize_t n = 0;
    size_t cnt = 0;
    TIC;
    size_t i, times = 1000;
    for (i = 0; i != times; i++) {
        while ((n = getline(&p, &len, fp)) > 0) {
            p[n - 1] = '\0';
            // printf("n=%ld len=%lu p='%s'\n", n, len, p);
            const char *s = Atom_string(p);
            (void)(s);
            cnt++;
            // break;
        }
        rewind(fp);
    }
    TOC;
    if (p != NULL) {
        free(p);
    }
    fclose(fp);
    return 0;
}

bool testAtomHash() {
    Atom_reset();
    char a[256];
    unsigned int i;
    for (i = 0; i != 255; i++) {
        a[i] = 'a';
        a[i + 1] = '\0';
        Atom_string(a);
    }
    Atom_debug();
    return true;
}

bool testAtomFree() {
    Atom_reset();
    const char *s1 = Atom_string("a");
    const char *s2 = Atom_string("aa");
    const char *s3 = Atom_string("aaa");
    Atom_debug();
    ASSERT(Atom_number() == 3);
    Atom_free(s2);
    Atom_debug();
    ASSERT(Atom_number() == 2);
    Atom_free(s1);
    Atom_debug();
    ASSERT(Atom_number() == 1);
    Atom_free(s3);
    Atom_debug();
    ASSERT(Atom_number() == 0);
    return true;
}

bool testAtom() {
    testAtomFree();
    testAtomHash();
    ASSERT(testAtomSimple());
    ASSERT(testAtomEq());
    perf_atom();
    Atom_debug();
    Atom_reset();
    Atom_debug();

    return true;
}
