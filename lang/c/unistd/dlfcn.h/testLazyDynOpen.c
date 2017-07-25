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
    {NULL, NULL},
};

#define MATHLIB     0
#define MAXLIB      1

DlSym dls[] = {
    {"/lib/x86_64-linux-gnu/libm.so.6", NULL, mathsyms, sizeof(mathsyms) / sizeof(FuncSym)},
    {NULL, NULL, NULL, 0},
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

int tryUnload() {
    for (int i = 0; dls[i].dlfn != NULL; i++) {
        if (dls[i].handle != NULL) {
            dlclose(dls[i].handle);
            Log("unload dl(%s)=%p\n", dls[i].dlfn, dls[i].handle);
            dls[i].handle = NULL;
        }

        FuncSym *syms = dls[i].syms;
        for (int j = 0; syms[j].symstr != NULL; j++) {
            syms[j].funcptr = NULL;
        }
    }
    return 0;
}

void *getFuncPtr(int libidx, int symidx) {
    if (libidx > sizeof(dls) / sizeof(DlSym)) {
        Log("libidx %d > %ld, out of range\n", libidx, sizeof(dls) / sizeof(DlSym));
        return NULL;
    }
    if (symidx > dls[libidx].symnum) {
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

double consine_wrap(double x) {
    double (*cosine)(double) = getFuncPtr(MATHLIB, COS_IDX);
    return cosine(x);
}

double sine_wrap(double x) {
    double (*sine)(double) = getFuncPtr(MATHLIB, SIN_IDX);
    return sine(x);
}

int main()
{
    tryUnload();
    Log("run symbol\n");
    printf("%f\n", sine_wrap(2.0));
    printf("%f\n", consine_wrap(2.0));

    tryUnload();
    return 0;
}
