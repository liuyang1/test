#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#define Log         printf

typedef struct {
    const char *symstr;
    void *funcptr;
} FuncSym;

typedef struct {
    const char *dlfn;
    void *handle;
    FuncSym *syms;
    const int symnum;
} DlSym;

#define COS_IDX     0
#define SIN_IDX     1
FuncSym mathsyms[] = {
    {"cos", NULL},
    {"sin", NULL},
};

#define MATHLIB     0
#define MAXLIB      1

DlSym dls[] = {
    {"/lib/x86_64-linux-gnu/libm.so.6", NULL, mathsyms, sizeof(mathsyms) / sizeof(FuncSym)},
};


int tryLoad(int libidx, int symidx) {
    char *error;

    int i = libidx;
    void *handle = dls[i].handle;
    if (handle == NULL) {
        if (dls[i].dlfn == NULL) {
            Log("%d have no dlfn\n", i);
            return -1;
        }
        const char *dlfn = dls[i].dlfn;
        handle = dlopen(dlfn, RTLD_LAZY);
        if (!handle) {
            fputs(dlerror(), stderr);
            exit(1);
        } else {
            Log("lazy-load dl(%s) -> handle=%p\n", dlfn, handle);
        }
        dls[i].handle = handle;
    }

    FuncSym *syms = dls[i].syms;
    int j = symidx;
    if (syms[j].funcptr == NULL) {
        if (syms[j].symstr == NULL) {
            Log("%d have no symstr\n", j);
            return -2;
        }

        const char *symstr = syms[j].symstr;
        syms[j].funcptr = dlsym(handle, symstr);
        if ((error = dlerror()) != NULL) {
            fputs(error, stderr);
            fputs("\n", stderr);
            exit(1);
        } else {
            Log("\tlazy-load %s=%p\n", syms[j].symstr, syms[j].funcptr);
        }
    }
    return 0;

}

void tryUnload() {
    for (int i = 0; i != sizeof(dls) / sizeof(dls[0]); i++) {
        if (dls[i].handle != NULL) {
            dlclose(dls[i].handle);
            Log("unload dl(%s)=%p\n", dls[i].dlfn, dls[i].handle);
            dls[i].handle = NULL;
        }

        FuncSym *syms = dls[i].syms;
        for (int j = 0; j != dls[i].symnum; j++) {
            syms[j].funcptr = NULL;
        }
    }
}

void *getFuncPtr(int libidx, int symidx) {
    if (libidx >= sizeof(dls) / sizeof(DlSym)) {
        Log("libidx %d > %ld, out of range\n", libidx, sizeof(dls) / sizeof(DlSym));
        return NULL;
    }
    if (symidx >= dls[libidx].symnum) {
        Log("symidx %d > %d, out of range\n", symidx, dls[libidx].symnum);
        return NULL;
    }
    void *funcptr = dls[libidx].syms[symidx].funcptr;
    if (funcptr == NULL) {
        tryLoad(libidx, symidx);
        funcptr = dls[libidx].syms[symidx].funcptr;
    }
    return funcptr;
}

// !!! REMIND !!!
// use SAME wrap function.
// This is RISKY as system's gcc may automaticly link libm.
//
// IF the library is not system's lib, use SAME wrap function is acceptable.
double cos(double x) {
    double (*fn)(double) = getFuncPtr(MATHLIB, COS_IDX);
    return fn(x);
}

// use different wrap function
double wrap_sin(double x) {
    printf("here\n");
    double (*fn)(double) = getFuncPtr(MATHLIB, SIN_IDX);
    return fn(x);
}

// directly calling function
double triple(double x) {
    return x * 3.0;
}


int main()
{
    // unload at exit, It will simplify unload process.
    atexit(tryUnload);

    printf("%f\n", wrap_sin(2.0));
    printf("%f\n", cos(2.0));
    printf("%f\n", triple(2.0));

    return 0;
}
